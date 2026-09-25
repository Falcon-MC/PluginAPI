package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

public abstract class DataPacketEvent extends Event {
    DataPacketEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public int packetId() {
        return api().eventPacketId(mHandle);
    }

    public MemorySegment dataView() {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment length = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment bytes = api().eventPacketData(mHandle, length);
            if (Interop.isNull(bytes)) {
                return MemorySegment.NULL;
            }
            return bytes.reinterpret(Integer.toUnsignedLong(length.get(ValueLayout.JAVA_INT, 0))).asReadOnly();
        }
    }

    public byte[] data() {
        MemorySegment view = dataView();
        if (Interop.isNull(view)) {
            return new byte[0];
        }
        return view.toArray(ValueLayout.JAVA_BYTE);
    }

    public void setData(byte[] data) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment bytes = MemorySegment.NULL;
            if (data.length > 0) {
                bytes = arena.allocateFrom(ValueLayout.JAVA_BYTE, data);
            }
            api().eventSetPacketData(mHandle, bytes, data.length);
        }
    }

    public void setData(MemorySegment data) {
        api().eventSetPacketData(mHandle, data, Math.toIntExact(data.byteSize()));
    }
}
