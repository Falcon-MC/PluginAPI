package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

/**
 * Two mobs in love make a baby; cancellable.
 */
public final class EntityBreedEvent extends Event {
    EntityBreedEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Optional<Entity> partner() {
        return eventTarget();
    }

    public String babyType() {
        return eventCause();
    }
}
