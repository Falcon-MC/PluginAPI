package falcon.api.event;

import falcon.api.Entity;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerInteractEntityEvent extends Event {
    PlayerInteractEntityEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Entity entity() {
        return eventEntity();
    }
}
