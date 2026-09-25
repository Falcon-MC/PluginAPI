package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerCommandEvent extends Event {
    PlayerCommandEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String command() {
        return eventMessage();
    }

    public void setCommand(String command) {
        eventSetMessage(command);
    }
}
