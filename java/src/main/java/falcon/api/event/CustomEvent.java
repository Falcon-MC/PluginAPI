package falcon.api.event;

import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;

public final class CustomEvent extends Event {
    public CustomEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public String name() {
        return Interop.string(api().eventName(mHandle));
    }

    public String data() {
        return Interop.string(api().eventData(mHandle));
    }

    public void setData(String data) {
        try (Arena arena = Arena.ofConfined()) {
            api().eventSetData(mHandle, Interop.text(arena, data));
        }
    }

    public String source() {
        return eventCause();
    }
}
