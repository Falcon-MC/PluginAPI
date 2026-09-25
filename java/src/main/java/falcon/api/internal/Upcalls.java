package falcon.api.internal;

import java.lang.foreign.Arena;
import java.lang.foreign.FunctionDescriptor;
import java.lang.foreign.Linker;
import java.lang.foreign.MemorySegment;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;

public final class Upcalls {
    private static final AtomicLong NEXT_ID = new AtomicLong(1);
    private static final Map<Long, Object> TARGETS = new ConcurrentHashMap<>();

    private Upcalls() {
    }

    public static MemorySegment stub(MethodHandles.Lookup lookup, String method, FunctionDescriptor descriptor) {
        try {
            MethodHandle target = lookup.findStatic(lookup.lookupClass(), method, descriptor.toMethodType());
            return Linker.nativeLinker().upcallStub(target, descriptor, Arena.global());
        } catch (ReflectiveOperationException exception) {
            throw new IllegalStateException("Cannot create the " + method + " callback", exception);
        }
    }

    public static MemorySegment register(Object target) {
        long id = NEXT_ID.getAndIncrement();
        TARGETS.put(id, target);
        return MemorySegment.ofAddress(id);
    }

    public static <T> T target(MemorySegment userData, Class<T> type) {
        Object target = TARGETS.get(userData.address());
        return type.isInstance(target) ? type.cast(target) : null;
    }

    public static <T> T release(MemorySegment userData, Class<T> type) {
        Object target = TARGETS.remove(userData.address());
        return type.isInstance(target) ? type.cast(target) : null;
    }
}
