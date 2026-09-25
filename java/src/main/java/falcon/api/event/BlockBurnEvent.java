package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Level;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class BlockBurnEvent extends Event {
    BlockBurnEvent(PluginContext context, MemorySegment handle) {
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

    public Vec3 source() {
        return eventPosition();
    }
}
