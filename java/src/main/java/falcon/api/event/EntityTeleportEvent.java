package falcon.api.event;

import falcon.api.Entity;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * An entity other than a player is moved by an end gateway, a pet following its owner or a plugin; cancellable.
 */
public final class EntityTeleportEvent extends Event {
    EntityTeleportEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Vec3 from() {
        return eventFrom();
    }

    public Vec3 to() {
        return eventTo();
    }

    public void setTo(Vec3 position) {
        eventSetTo(position);
    }

    public String cause() {
        return eventCause();
    }
}
