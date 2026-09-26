package falcon.api.event;

import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class ItemDamageEvent extends Event {
    ItemDamageEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Item item() {
        return eventItem();
    }

    public int damage() {
        return (int) eventAmount();
    }

    public void setDamage(int damage) {
        eventSetAmount(damage);
    }
}
