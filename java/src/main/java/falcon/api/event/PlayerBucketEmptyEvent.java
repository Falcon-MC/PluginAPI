package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A player empties a bucket into the world or a cauldron; cancellable. The block is where the content goes.
 */
public final class PlayerBucketEmptyEvent extends Event {
    PlayerBucketEmptyEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Item bucket() {
        return eventItem();
    }

    public BlockPos block() {
        return eventBlockPosition();
    }

    public String blockName() {
        return eventBlockName();
    }
}
