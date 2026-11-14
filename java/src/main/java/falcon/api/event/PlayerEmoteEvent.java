package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A player plays an emote; cancellable, the others then do not see it.
 */
public final class PlayerEmoteEvent extends Event {
    PlayerEmoteEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String emote() {
        return eventMessage();
    }
}
