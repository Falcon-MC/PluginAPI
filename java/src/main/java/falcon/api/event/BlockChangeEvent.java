package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public abstract class BlockChangeEvent extends Event {
    BlockChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public String blockName() {
        return eventBlockName();
    }

    public String previousBlockName() {
        return eventPreviousBlockName();
    }
}
