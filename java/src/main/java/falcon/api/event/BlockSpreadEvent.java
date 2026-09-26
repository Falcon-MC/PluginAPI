package falcon.api.event;

import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class BlockSpreadEvent extends BlockChangeEvent {
    BlockSpreadEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Vec3 source() {
        return eventFrom();
    }
}
