package falcon.api.event;

import falcon.api.Player;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerMoveEvent extends Event {
    PlayerMoveEvent(PluginContext context, MemorySegment handle) {
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
}
