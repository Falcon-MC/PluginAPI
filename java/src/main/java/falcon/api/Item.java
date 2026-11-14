package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public final class Item implements AutoCloseable {
    private MemorySegment mHandle;
    private final boolean mOwned;

    private Item(MemorySegment handle, boolean owned) {
        mHandle = handle;
        mOwned = owned;
    }

    public static Optional<Item> create(String identifier) {
        return create(identifier, 1);
    }

    public static Optional<Item> create(String identifier, int count) {
        try (Arena arena = Arena.ofConfined()) {
            return adopt(api().itemCreate(Interop.text(arena, identifier), count));
        }
    }

    public static Optional<Item> adopt(MemorySegment handle) {
        if (Interop.isNull(handle)) {
            return Optional.empty();
        }
        return Optional.of(new Item(handle, true));
    }

    // an empty stack is freed at once: nobody would close it
    static Optional<Item> adoptFilled(MemorySegment handle) {
        Optional<Item> item = adopt(handle);
        if (item.isPresent() && item.get().isEmpty()) {
            item.get().close();
            return Optional.empty();
        }
        return item;
    }

    public static Item borrow(MemorySegment handle) {
        return new Item(handle == null ? MemorySegment.NULL : handle, false);
    }

    /**
     * The item's tag as little-endian NBT, the game's save format; empty for an item without one.
     */
    public byte[] nbt() {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment length = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment data = api().itemNbt(mHandle, length);
            int size = length.get(ValueLayout.JAVA_INT, 0);
            if (Interop.isNull(data) || size <= 0) {
                return new byte[0];
            }
            return data.reinterpret(size).toArray(ValueLayout.JAVA_BYTE);
        }
    }

    /**
     * Replaces the item's tag; empty data removes it. Returns false when the data is not a little-endian NBT
     * compound.
     */
    public boolean setNbt(byte[] data) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment bytes = data.length == 0 ? MemorySegment.NULL : arena.allocateFrom(ValueLayout.JAVA_BYTE,
                    data);
            return api().itemSetNbt(mHandle, bytes, data.length) != 0;
        }
    }

    public Optional<Item> copy() {
        if (Interop.isNull(mHandle)) {
            return Optional.empty();
        }
        return adopt(api().itemCopy(mHandle));
    }

    @Override
    public void close() {
        if (!mOwned || Interop.isNull(mHandle)) {
            return;
        }
        api().itemDestroy(mHandle);
        mHandle = MemorySegment.NULL;
    }

    public boolean valid() {
        return !Interop.isNull(mHandle);
    }

    public boolean owned() {
        return mOwned;
    }

    public boolean isEmpty() {
        return Interop.isNull(mHandle) || api().itemIsEmpty(mHandle) != 0;
    }

    public String identifier() {
        return Interop.string(api().itemIdentifier(mHandle));
    }

    public int count() {
        return api().itemCount(mHandle);
    }

    public void setCount(int count) {
        api().itemSetCount(mHandle, count);
    }

    public int damage() {
        return api().itemDamage(mHandle);
    }

    public void setDamage(int damage) {
        api().itemSetDamage(mHandle, damage);
    }

    public String customName() {
        return Interop.string(api().itemCustomName(mHandle));
    }

    public void setCustomName(String name) {
        try (Arena arena = Arena.ofConfined()) {
            api().itemSetCustomName(mHandle, Interop.text(arena, name));
        }
    }

    public List<String> lore() {
        int count = api().itemLoreCount(mHandle);
        List<String> lines = new ArrayList<>(count);
        for (int index = 0; index < count; index++) {
            lines.add(Interop.string(api().itemLore(mHandle, index)));
        }
        return lines;
    }

    public void setLore(List<String> lines) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment values = MemorySegment.NULL;
            if (!lines.isEmpty()) {
                values = arena.allocate(ValueLayout.ADDRESS, lines.size());
                for (int index = 0; index < lines.size(); index++) {
                    values.setAtIndex(ValueLayout.ADDRESS, index, Interop.text(arena, lines.get(index)));
                }
            }
            api().itemSetLore(mHandle, values, lines.size());
        }
    }

    public int enchantmentLevel(int enchantment) {
        return api().itemEnchantmentLevel(mHandle, enchantment);
    }

    public void setEnchantmentLevel(int enchantment, int level) {
        api().itemSetEnchantmentLevel(mHandle, enchantment, level);
    }

    public MemorySegment handle() {
        return mHandle;
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
