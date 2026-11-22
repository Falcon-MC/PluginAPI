package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerQuitEvent extends Event {
    PlayerQuitEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    /**
     * Why the player left: the kick reason, or what the connection gave.
     */
    public String reason() {
        return eventCause();
    }

    /**
     * The leave message everyone sees, in English while it is still the game's own.
     */
    public String message() {
        return eventMessage();
    }

    /**
     * Replaces the leave message; an empty one sends none.
     */
    public void setMessage(String message) {
        eventSetMessage(message);
    }

    /**
     * True while the message is the game's own, which each client shows in its language.
     */
    public boolean messageIsDefault() {
        return eventMessageIsDefault();
    }
}
