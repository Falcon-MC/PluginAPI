package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A player fills a bucket from a liquid, a cauldron or powder snow; cancellable.
 */
public final class PlayerBucketFillEvent extends Event {
    PlayerBucketFillEvent(PluginContext context, MemorySegment handle) {
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
