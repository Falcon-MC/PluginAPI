package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class BlockPlaceEvent extends Event {
    BlockPlaceEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public String blockName() {
        return eventBlockName();
    }
}
