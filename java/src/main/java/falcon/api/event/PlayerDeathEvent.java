package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerDeathEvent extends Event {
    PlayerDeathEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String message() {
        return eventMessage();
    }

    public void setMessage(String message) {
        eventSetMessage(message);
    }
}
