package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class LeavesDecayEvent extends BlockChangeEvent {
    LeavesDecayEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }
}
