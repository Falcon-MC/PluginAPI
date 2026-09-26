package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

public final class ProjectileLaunchEvent extends Event {
    ProjectileLaunchEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity projectile() {
        return eventEntity();
    }

    public Optional<Entity> shooter() {
        return eventAttacker();
    }
}
