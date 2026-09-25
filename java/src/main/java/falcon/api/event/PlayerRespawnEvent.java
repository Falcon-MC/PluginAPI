package falcon.api.event;

import falcon.api.Player;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerRespawnEvent extends Event {
    PlayerRespawnEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Vec3 position() {
        return eventTo();
    }

    public void setPosition(Vec3 position) {
        eventSetTo(position);
    }
}
