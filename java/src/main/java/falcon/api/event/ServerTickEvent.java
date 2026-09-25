package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class ServerTickEvent extends Event {
    ServerTickEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public long tick() {
        return api().eventTick(mHandle);
    }
}
