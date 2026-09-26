package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class BlockFormEvent extends BlockChangeEvent {
    BlockFormEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }
}
