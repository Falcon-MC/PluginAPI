package falcon.api.event;

import falcon.api.Item;
import falcon.api.Level;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * An item entity is about to appear in the world; cancellable.
 */
public final class ItemSpawnEvent extends Event {
    ItemSpawnEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public Item item() {
        return eventItem();
    }

    public Vec3 position() {
        return eventPosition();
    }
}
