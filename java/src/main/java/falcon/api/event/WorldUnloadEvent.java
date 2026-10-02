package falcon.api.event;

import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class WorldUnloadEvent extends Event {
    WorldUnloadEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public String world() {
        return Interop.string(api().eventWorldName(mHandle));
    }
}
