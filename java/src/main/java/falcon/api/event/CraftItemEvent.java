package falcon.api.event;

import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class CraftItemEvent extends Event {
    CraftItemEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Item result() {
        return eventItem();
    }

    public int times() {
        return (int) api().eventAmount(mHandle);
    }
}
