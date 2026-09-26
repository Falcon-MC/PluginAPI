package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.List;

public abstract class PistonEvent extends Event {
    PistonEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public int face() {
        return eventBlockFace();
    }

    public List<BlockPos> blocks() {
        return eventBlocks();
    }
}
