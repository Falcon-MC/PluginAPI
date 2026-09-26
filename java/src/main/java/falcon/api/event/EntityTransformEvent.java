package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

public final class EntityTransformEvent extends Event {
    EntityTransformEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Optional<Entity> result() {
        return eventTarget();
    }
}
