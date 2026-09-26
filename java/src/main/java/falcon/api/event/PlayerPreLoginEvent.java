package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerPreLoginEvent extends Event {
    PlayerPreLoginEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String kickMessage() {
        return eventMessage();
    }

    public void setKickMessage(String message) {
        eventSetMessage(message);
    }
}
