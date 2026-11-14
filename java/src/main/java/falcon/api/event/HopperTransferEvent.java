package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A hopper moves one item into the container it faces (pushing) or out of the one above; cancellable.
 */
public final class HopperTransferEvent extends Event {
    HopperTransferEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos hopper() {
        return eventBlockPosition();
    }

    public Item item() {
        return eventItem();
    }

    public boolean pushing() {
        return eventState();
    }
}
