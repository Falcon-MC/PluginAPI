package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Level;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class InventoryOpenEvent extends Event {
    InventoryOpenEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
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
}
