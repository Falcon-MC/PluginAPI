package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.internal.Upcalls;

import java.lang.foreign.MemorySegment;
import java.lang.invoke.MethodHandles;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;

public final class Scheduler {
    private static final MethodHandles.Lookup LOOKUP = MethodHandles.lookup();
    private static final MemorySegment RUN_ONCE = Upcalls.stub(LOOKUP, "runOnce", FalconAbi.TASK);
    private static final MemorySegment RUN_REPEATING = Upcalls.stub(LOOKUP, "runRepeating", FalconAbi.TASK);
    private static final MemorySegment RUN_WORK = Upcalls.stub(LOOKUP, "runWork", FalconAbi.TASK);
    private static final MemorySegment RUN_DONE = Upcalls.stub(LOOKUP, "runDone", FalconAbi.TASK);

    private final PluginContext mContext;
    private final Map<Long, MemorySegment> mTasks = new ConcurrentHashMap<>();

    Scheduler(PluginContext context) {
        mContext = context;
    }

    public long delay(long ticks, Runnable task) {
        return schedule(RUN_ONCE, ticks, 0, task);
    }

    public long repeat(long periodTicks, Runnable task) {
        return schedule(RUN_REPEATING, periodTicks, periodTicks, task);
    }

    public long async(Runnable work) {
        return async(work, null);
    }

    public long async(Runnable work, Runnable done) {
        Objects.requireNonNull(work, "work");
        MemorySegment userData = Upcalls.register(new AsyncJob(mContext, work, done));
        long id = Interop.api().runAsync(mContext.handle(), RUN_WORK, RUN_DONE, userData);
        if (id == 0) {
            Upcalls.release(userData, AsyncJob.class);
        }
        return id;
    }

    public void cancel(long task) {
        Interop.api().cancelTask(task);
        MemorySegment userData = mTasks.remove(task);
        if (userData != null) {
            Upcalls.release(userData, Task.class);
        }
    }

    private long schedule(MemorySegment runner, long delayTicks, long periodTicks, Runnable action) {
        Objects.requireNonNull(action, "task");
        Task task = new Task(this, mContext, action);
        MemorySegment userData = Upcalls.register(task);
        long id = Interop.api().scheduleTask(mContext.handle(), runner, userData, delayTicks, periodTicks);
        if (id == 0) {
            Upcalls.release(userData, Task.class);
            return 0;
        }
        task.mId = id;
        mTasks.put(id, userData);
        return id;
    }

    private static void runOnce(MemorySegment userData) {
        try {
            Task task = Upcalls.release(userData, Task.class);
            if (task != null) {
                task.mOwner.mTasks.remove(task.mId);
                task.mContext.guard("A task threw an exception", task.mAction);
            }
        } catch (Throwable ignored) {
        }
    }

    private static void runRepeating(MemorySegment userData) {
        try {
            Task task = Upcalls.target(userData, Task.class);
            if (task != null) {
                task.mContext.guard("A task threw an exception", task.mAction);
            }
        } catch (Throwable ignored) {
        }
    }

    private static void runWork(MemorySegment userData) {
        try {
            AsyncJob job = Upcalls.target(userData, AsyncJob.class);
            if (job != null) {
                job.context().guard("An asynchronous task threw an exception", job.work());
            }
        } catch (Throwable ignored) {
        }
    }

    private static void runDone(MemorySegment userData) {
        try {
            AsyncJob job = Upcalls.release(userData, AsyncJob.class);
            if (job != null && job.done() != null) {
                job.context().guard("An asynchronous callback threw an exception", job.done());
            }
        } catch (Throwable ignored) {
        }
    }

    private static final class Task {
        private final Scheduler mOwner;
        private final PluginContext mContext;
        private final Runnable mAction;
        private volatile long mId;

        private Task(Scheduler owner, PluginContext context, Runnable action) {
            mOwner = owner;
            mContext = context;
            mAction = action;
        }
    }

    private record AsyncJob(PluginContext context, Runnable work, Runnable done) {
    }
}
