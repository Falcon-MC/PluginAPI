package falcon.api.internal;

import falcon.api.Plugin;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.function.BooleanSupplier;

public final class PluginContext {
    private static final ThreadLocal<PluginContext> PENDING = new ThreadLocal<>();

    private final MemorySegment mHandle;
    private final String mName;
    private volatile ClassLoader mClassLoader;
    private volatile Plugin mPlugin;

    PluginContext(MemorySegment handle, String name) {
        mHandle = handle;
        mName = name;
    }

    public static PluginContext claim() {
        PluginContext context = PENDING.get();
        if (context == null) {
            throw new IllegalStateException("Plugins are created by the server");
        }
        PENDING.remove();
        return context;
    }

    static void offer(PluginContext context) {
        PENDING.set(context);
    }

    static void withdraw() {
        PENDING.remove();
    }

    void attach(ClassLoader classLoader, Plugin plugin) {
        mClassLoader = classLoader;
        mPlugin = plugin;
    }

    Plugin plugin() {
        return mPlugin;
    }

    public MemorySegment handle() {
        return mHandle;
    }

    public String name() {
        return mName;
    }

    public void log(int level, String message) {
        try (Arena arena = Arena.ofConfined()) {
            Interop.api().log(mHandle, level, Interop.text(arena, message));
        }
    }

    public void report(String where, Throwable throwable) {
        try {
            StringWriter trace = new StringWriter();
            throwable.printStackTrace(new PrintWriter(trace));
            log(FalconAbi.LOG_ERROR, where + ": " + trace.toString().stripTrailing());
        } catch (Throwable ignored) {
        }
    }

    public void guard(String where, Runnable action) {
        ClassLoader previous = enter();
        try {
            action.run();
        } catch (Throwable throwable) {
            report(where, throwable);
        } finally {
            leave(previous);
        }
    }

    public boolean test(String where, BooleanSupplier action) {
        ClassLoader previous = enter();
        try {
            return action.getAsBoolean();
        } catch (Throwable throwable) {
            report(where, throwable);
            return false;
        } finally {
            leave(previous);
        }
    }

    private ClassLoader enter() {
        Thread thread = Thread.currentThread();
        ClassLoader previous = thread.getContextClassLoader();
        ClassLoader own = mClassLoader;
        if (own != null) {
            thread.setContextClassLoader(own);
        }
        return previous;
    }

    private static void leave(ClassLoader previous) {
        Thread.currentThread().setContextClassLoader(previous);
    }
}
