package falcon.api.service;

import falcon.api.event.CustomEvent;
import falcon.api.event.EventPriority;
import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.internal.Upcalls;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandles;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Consumer;

public final class Services {
    private static final MethodHandles.Lookup LOOKUP = MethodHandles.lookup();
    private static final MemorySegment SERVE = Upcalls.stub(LOOKUP, "serve", FalconAbi.SERVICE_HANDLER);
    private static final MemorySegment DISPATCH = Upcalls.stub(LOOKUP, "dispatch", FalconAbi.EVENT_HANDLER);
    private static final ThreadLocal<Arena> RESPONSES = new ThreadLocal<>();

    private final PluginContext mContext;
    private final Map<String, MemorySegment> mProvided = new ConcurrentHashMap<>();

    public Services(PluginContext context) {
        mContext = context;
    }

    public boolean provide(String name, ServiceHandler handler) {
        Objects.requireNonNull(handler, "handler");
        MemorySegment userData = Upcalls.register(new Provider(mContext, handler));
        try (Arena arena = Arena.ofConfined()) {
            boolean provided = Interop.api().registerService(mContext.handle(), Interop.text(arena, name), SERVE,
                    userData) != 0;
            if (!provided) {
                Upcalls.release(userData, Provider.class);
                return false;
            }
        }

        MemorySegment previous = mProvided.put(name, userData);
        if (previous != null) {
            Upcalls.release(previous, Provider.class);
        }
        return true;
    }

    public void withdraw(String name) {
        try (Arena arena = Arena.ofConfined()) {
            Interop.api().unregisterService(mContext.handle(), Interop.text(arena, name));
        }
        MemorySegment userData = mProvided.remove(name);
        if (userData != null) {
            Upcalls.release(userData, Provider.class);
        }
    }

    public boolean has(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.api().hasService(Interop.text(arena, name)) != 0;
        }
    }

    public Optional<String> provider(String name) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment plugin = Interop.api().serviceProvider(Interop.text(arena, name));
            if (Interop.isNull(plugin)) {
                return Optional.empty();
            }
            return Optional.of(Interop.string(plugin));
        }
    }

    public Optional<String> call(String name) {
        return call(name, "");
    }

    public Optional<String> call(String name, String request) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment found = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment response = Interop.api().callService(Interop.text(arena, name),
                    Interop.text(arena, request), found);
            if (found.get(ValueLayout.JAVA_INT, 0) == 0) {
                return Optional.empty();
            }
            return Optional.of(Interop.string(response));
        }
    }

    public long on(String name, Consumer<? super CustomEvent> handler) {
        return on(name, handler, EventPriority.NORMAL, false);
    }

    public long on(String name, Consumer<? super CustomEvent> handler, EventPriority priority) {
        return on(name, handler, priority, false);
    }

    public long on(String name, Consumer<? super CustomEvent> handler, EventPriority priority,
                   boolean ignoreCancelled) {
        Objects.requireNonNull(handler, "handler");
        MemorySegment userData = Upcalls.register(new Listener(mContext, handler));
        try (Arena arena = Arena.ofConfined()) {
            long id = Interop.api().subscribeCustomEvent(mContext.handle(), Interop.text(arena, name),
                    priority.value(), ignoreCancelled ? 1 : 0, DISPATCH, userData);
            if (id == 0) {
                Upcalls.release(userData, Listener.class);
            }
            return id;
        }
    }

    public CustomEventResult fire(String name) {
        return fire(name, "", false);
    }

    public CustomEventResult fire(String name, String data) {
        return fire(name, data, false);
    }

    public CustomEventResult fire(String name, String data, boolean cancellable) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment cancelled = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment result = Interop.api().fireCustomEvent(mContext.handle(), Interop.text(arena, name),
                    Interop.text(arena, data), cancellable ? 1 : 0, cancelled);
            return new CustomEventResult(cancelled.get(ValueLayout.JAVA_INT, 0) != 0, Interop.string(result));
        }
    }

    private static MemorySegment serve(MemorySegment request, MemorySegment userData) {
        try {
            Provider provider = Upcalls.target(userData, Provider.class);
            String text = Interop.string(request);
            String[] response = {""};
            if (provider != null) {
                provider.context().guard("A service handler threw an exception", () -> {
                    response[0] = provider.handler().handle(text);
                });
            }
            return respond(response[0]);
        } catch (Throwable ignored) {
            return MemorySegment.NULL;
        }
    }

    private static MemorySegment respond(String response) {
        Arena previous = RESPONSES.get();
        if (previous != null) {
            previous.close();
        }
        Arena arena = Arena.ofConfined();
        RESPONSES.set(arena);
        return Interop.text(arena, response);
    }

    private static void dispatch(MemorySegment event, MemorySegment userData) {
        try {
            Listener listener = Upcalls.target(userData, Listener.class);
            if (listener != null) {
                CustomEvent custom = new CustomEvent(listener.context(), event);
                listener.context().guard("A custom event handler threw an exception",
                        () -> listener.handler().accept(custom));
            }
        } catch (Throwable ignored) {
        }
    }

    private record Provider(PluginContext context, ServiceHandler handler) {
    }

    private record Listener(PluginContext context, Consumer<? super CustomEvent> handler) {
    }
}
