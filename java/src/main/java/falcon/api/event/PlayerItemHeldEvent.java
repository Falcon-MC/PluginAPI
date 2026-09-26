package falcon.api.event;

import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerItemHeldEvent extends Event {
    PlayerItemHeldEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public int previousSlot() {
        return eventSourceSlot();
    }

    public int newSlot() {
        return eventDestinationSlot();
    }

    public Item item() {
        return eventItem();
    }
}
