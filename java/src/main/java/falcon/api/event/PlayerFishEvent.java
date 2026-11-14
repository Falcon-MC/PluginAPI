package falcon.api.event;

import falcon.api.Entity;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

/**
 * A player reels in a hook that caught a fish or holds an entity; cancellable.
 */
public final class PlayerFishEvent extends Event {
    PlayerFishEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Entity hook() {
        return eventEntity();
    }

    public Optional<Entity> caught() {
        return eventTarget();
    }

    public boolean caughtFish() {
        return eventState();
    }
}
