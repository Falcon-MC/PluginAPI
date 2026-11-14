package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

/**
 * A mob is tamed; cancellable.
 */
public final class EntityTameEvent extends Event {
    EntityTameEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Optional<Entity> tamer() {
        return eventTarget();
    }
}
