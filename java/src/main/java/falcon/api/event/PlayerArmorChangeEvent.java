package falcon.api.event;

import falcon.api.ArmorSlot;
import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * An armor slot of a player changed through the inventory or by equipping the held item; not cancellable.
 */
public final class PlayerArmorChangeEvent extends Event {
    PlayerArmorChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public ArmorSlot slot() {
        return ArmorSlot.of(eventDestinationSlot());
    }

    public Item item() {
        return eventItem();
    }

    public Item previousItem() {
        return eventResult();
    }
}
