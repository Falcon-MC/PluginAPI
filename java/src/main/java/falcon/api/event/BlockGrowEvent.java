package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class BlockGrowEvent extends BlockChangeEvent {
    BlockGrowEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }
}
