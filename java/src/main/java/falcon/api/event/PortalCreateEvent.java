package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.List;

/**
 * A nether portal lights up; cancellable. The blocks are the portal blocks.
 */
public final class PortalCreateEvent extends Event {
    PortalCreateEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public List<BlockPos> blocks() {
        return eventBlocks();
    }
}
