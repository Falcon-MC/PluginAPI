package falcon.api.event;

import falcon.api.Entity;
import falcon.api.Level;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class EntitySpawnEvent extends Event {
    EntitySpawnEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Level level() {
        return eventLevel();
    }

    public Vec3 position() {
        return eventPosition();
    }
}
