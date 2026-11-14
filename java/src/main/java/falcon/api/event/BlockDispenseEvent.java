package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A dispenser or dropper fires an item; cancellable.
 */
public final class BlockDispenseEvent extends Event {
    BlockDispenseEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos block() {
        return eventBlockPosition();
    }

    public String blockName() {
        return eventBlockName();
    }

    public Item item() {
        return eventItem();
    }
}
