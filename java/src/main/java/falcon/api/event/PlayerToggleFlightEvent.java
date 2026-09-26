package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerToggleFlightEvent extends Event {
    PlayerToggleFlightEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public boolean isFlying() {
        return eventState();
    }
}
