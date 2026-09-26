package falcon.api.event;

import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class ItemBreakEvent extends Event {
    ItemBreakEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Item item() {
        return eventItem();
    }
}
