package falcon.api.event;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.internal.Upcalls;

import java.lang.foreign.MemorySegment;
import java.lang.invoke.MethodHandles;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Consumer;

public final class Events {
    private static final MemorySegment DISPATCH =
            Upcalls.stub(MethodHandles.lookup(), "dispatch", FalconAbi.EVENT_HANDLER);

    private final PluginContext mContext;
    private final Map<Long, MemorySegment> mSubscriptions = new ConcurrentHashMap<>();

    public Events(PluginContext context) {
        mContext = context;
    }

    public <T extends Event> long on(Class<T> type, Consumer<? super T> handler) {
        return on(type, handler, EventPriority.NORMAL, false);
    }

    public <T extends Event> long on(Class<T> type, Consumer<? super T> handler, EventPriority priority) {
        return on(type, handler, priority, false);
    }

    public <T extends Event> long on(Class<T> type, Consumer<? super T> handler, EventPriority priority,
                                     boolean ignoreCancelled) {
        Objects.requireNonNull(handler, "handler");
        EventType<T> eventType = EventType.of(type);
        MemorySegment userData = Upcalls.register(new Subscription<>(mContext, eventType, handler));
        long id = Interop.api().subscribe(mContext.handle(), eventType.id(), priority.value(), ignoreCancelled ? 1 : 0,
                DISPATCH, userData);
        return remember(id, userData);
    }

    public void off(long subscription) {
        Interop.api().unsubscribe(subscription);
        MemorySegment userData = mSubscriptions.remove(subscription);
        if (userData != null) {
            Upcalls.release(userData, Subscription.class);
        }
    }

    private long remember(long id, MemorySegment userData) {
        if (id == 0) {
            Upcalls.release(userData, Subscription.class);
            return 0;
        }
        mSubscriptions.put(id, userData);
        return id;
    }

    private static void dispatch(MemorySegment event, MemorySegment userData) {
        try {
            Subscription<?> subscription = Upcalls.target(userData, Subscription.class);
            if (subscription != null) {
                subscription.fire(event);
            }
        } catch (Throwable ignored) {
        }
    }

    private record Subscription<T extends Event>(PluginContext context, EventType<T> type,
                                                 Consumer<? super T> handler) {
        void fire(MemorySegment event) {
            context.guard("An event handler threw an exception", () -> handler.accept(type.create(context, event)));
        }
    }
}
