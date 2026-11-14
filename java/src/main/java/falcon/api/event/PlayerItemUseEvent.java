package falcon.api.event;

import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A player uses the held item in the air; cancellable. The item is a copy.
 */
public final class PlayerItemUseEvent extends Event {
    PlayerItemUseEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Item item() {
        return eventItem();
    }
}
