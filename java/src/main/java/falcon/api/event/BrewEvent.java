package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A brewing stand finishes brewing; cancellable.
 */
public final class BrewEvent extends Event {
    BrewEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos block() {
        return eventBlockPosition();
    }

    public Item ingredient() {
        return eventItem();
    }
}
