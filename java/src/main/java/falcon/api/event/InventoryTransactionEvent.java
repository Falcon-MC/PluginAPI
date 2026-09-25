package falcon.api.event;

import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class InventoryTransactionEvent extends Event {
    InventoryTransactionEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String action() {
        return eventCause();
    }

    public Item item() {
        return eventItem();
    }

    public int count() {
        return (int) api().eventAmount(mHandle);
    }

    public String sourceContainer() {
        return Interop.string(api().eventSourceContainer(mHandle));
    }

    public int sourceSlot() {
        return api().eventSourceSlot(mHandle);
    }

    public String destinationContainer() {
        return Interop.string(api().eventDestinationContainer(mHandle));
    }

    public int destinationSlot() {
        return api().eventDestinationSlot(mHandle);
    }
}
