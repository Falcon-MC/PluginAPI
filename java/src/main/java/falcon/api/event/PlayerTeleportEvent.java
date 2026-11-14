package falcon.api.event;

import falcon.api.Player;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A player is moved by a command, an ender pearl, a chorus fruit, an end gateway or a plugin; cancellable, the
 * destination can change.
 */
public final class PlayerTeleportEvent extends Event {
    PlayerTeleportEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
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
