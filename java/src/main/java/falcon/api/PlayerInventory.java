package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

public final class PlayerInventory {
    private final MemorySegment mPlayer;

    public PlayerInventory(MemorySegment player) {
        mPlayer = player;
    }

    public int size() {
        return api().playerInventorySize(mPlayer);
    }

    public Optional<Item> item(int slot) {
        return Item.adopt(api().playerInventoryItem(mPlayer, slot));
    }

    public void setItem(int slot, Item item) {
        api().playerSetInventoryItem(mPlayer, slot, item.handle());
    }

    public boolean give(Item item) {
        return api().playerGiveItem(mPlayer, item.handle()) != 0;
    }

    public int selectedSlot() {
        return api().playerSelectedSlot(mPlayer);
    }

    public void setSelectedSlot(int slot) {
        api().playerSetSelectedSlot(mPlayer, slot);
    }

    public Optional<Item> heldItem() {
        return item(selectedSlot());
    }

    public Optional<Item> armor(ArmorSlot slot) {
        return Item.adopt(api().playerArmorItem(mPlayer, slot.value()));
    }

    public void setArmor(ArmorSlot slot, Item item) {
        api().playerSetArmorItem(mPlayer, slot.value(), item.handle());
    }

    public Optional<Item> offhand() {
        return Item.adopt(api().playerOffhandItem(mPlayer));
    }

    public void setOffhand(Item item) {
        api().playerSetOffhandItem(mPlayer, item.handle());
    }

    public void clear() {
        api().playerClearInventory(mPlayer);
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
