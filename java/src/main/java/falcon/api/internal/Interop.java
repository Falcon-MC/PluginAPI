package falcon.api.internal;

import falcon.api.BlockPos;
import falcon.api.Vec3;

import java.lang.foreign.MemorySegment;
import java.lang.foreign.SegmentAllocator;
import java.lang.foreign.ValueLayout;

public final class Interop {
    private static volatile NativeApi sApi;

    private Interop() {
    }

    public static NativeApi api() {
        NativeApi api = sApi;
        if (api == null) {
            throw new IllegalStateException("The server API is not bound yet");
        }
        return api;
    }

    static synchronized NativeApi bind(MemorySegment table) {
        if (sApi == null) {
            sApi = new NativeApi(table);
        }
        return sApi;
    }

    public static boolean isNull(MemorySegment pointer) {
        return pointer == null || pointer.address() == 0;
    }

    public static String string(MemorySegment pointer) {
        if (isNull(pointer)) {
            return "";
        }
        return pointer.reinterpret(Long.MAX_VALUE).getString(0);
    }

    public static MemorySegment text(SegmentAllocator allocator, String value) {
        return allocator.allocateFrom(value == null ? "" : value);
    }

    public static MemorySegment nullableText(SegmentAllocator allocator, String value) {
        if (value == null || value.isEmpty()) {
            return MemorySegment.NULL;
        }
        return allocator.allocateFrom(value);
    }

    public static MemorySegment vec3(SegmentAllocator allocator, Vec3 value) {
        MemorySegment segment = allocator.allocate(FalconAbi.Vec3.LAYOUT);
        segment.set(ValueLayout.JAVA_DOUBLE, FalconAbi.Vec3.X, value.x());
        segment.set(ValueLayout.JAVA_DOUBLE, FalconAbi.Vec3.Y, value.y());
        segment.set(ValueLayout.JAVA_DOUBLE, FalconAbi.Vec3.Z, value.z());
        return segment;
    }

    public static Vec3 vec3(MemorySegment segment) {
        return new Vec3(segment.get(ValueLayout.JAVA_DOUBLE, FalconAbi.Vec3.X),
                segment.get(ValueLayout.JAVA_DOUBLE, FalconAbi.Vec3.Y),
                segment.get(ValueLayout.JAVA_DOUBLE, FalconAbi.Vec3.Z));
    }

    public static MemorySegment blockPos(SegmentAllocator allocator, BlockPos value) {
        MemorySegment segment = allocator.allocate(FalconAbi.BlockPos.LAYOUT);
        writeBlockPos(segment, value);
        return segment;
    }

    public static void writeBlockPos(MemorySegment segment, BlockPos value) {
        segment.set(ValueLayout.JAVA_INT, FalconAbi.BlockPos.X, value.x());
        segment.set(ValueLayout.JAVA_INT, FalconAbi.BlockPos.Y, value.y());
        segment.set(ValueLayout.JAVA_INT, FalconAbi.BlockPos.Z, value.z());
    }

    public static BlockPos blockPos(MemorySegment segment) {
        return new BlockPos(segment.get(ValueLayout.JAVA_INT, FalconAbi.BlockPos.X),
                segment.get(ValueLayout.JAVA_INT, FalconAbi.BlockPos.Y),
                segment.get(ValueLayout.JAVA_INT, FalconAbi.BlockPos.Z));
    }
}
