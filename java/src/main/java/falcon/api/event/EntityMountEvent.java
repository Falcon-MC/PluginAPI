package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

/**
 * An entity or player gets on a vehicle; cancellable.
 */
public final class EntityMountEvent extends Event {
    EntityMountEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Optional<Entity> vehicle() {
        return eventTarget();
    }
}
