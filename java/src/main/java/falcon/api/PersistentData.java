package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

/**
 * Text this plugin keeps on entities, players and items, saved with them. Keys belong to the plugin: another plugin
 * never sees them. Entities without saved state, such as dropped items, keep nothing, and a copy of an inventory
 * item must be put back for its change to stay.
 */
public final class PersistentData {
    private final PluginContext mContext;

    PersistentData(PluginContext context) {
        mContext = context;
    }

    public Optional<String> get(Entity entity, String key) {
        try (Arena arena = Arena.ofConfined()) {
            return optional(api().entityData(mContext.handle(), entity.handle(), Interop.text(arena, key)));
        }
    }

    public boolean set(Entity entity, String key, String value) {
        try (Arena arena = Arena.ofConfined()) {
            return api().entitySetData(mContext.handle(), entity.handle(), Interop.text(arena, key),
                    Interop.text(arena, value)) != 0;
        }
    }

    public boolean remove(Entity entity, String key) {
        try (Arena arena = Arena.ofConfined()) {
            return api().entitySetData(mContext.handle(), entity.handle(), Interop.text(arena, key),
                    MemorySegment.NULL) != 0;
        }
    }

    public List<String> keys(Entity entity) {
        int count = api().entityDataCount(mContext.handle(), entity.handle());
        List<String> keys = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            keys.add(Interop.string(api().entityDataKey(mContext.handle(), entity.handle(), index)));
        }
        return keys;
    }

    public Optional<String> get(Item item, String key) {
        try (Arena arena = Arena.ofConfined()) {
            return optional(api().itemData(mContext.handle(), item.handle(), Interop.text(arena, key)));
        }
    }

    public boolean set(Item item, String key, String value) {
        try (Arena arena = Arena.ofConfined()) {
            return api().itemSetData(mContext.handle(), item.handle(), Interop.text(arena, key),
                    Interop.text(arena, value)) != 0;
        }
    }

    public boolean remove(Item item, String key) {
        try (Arena arena = Arena.ofConfined()) {
            return api().itemSetData(mContext.handle(), item.handle(), Interop.text(arena, key),
                    MemorySegment.NULL) != 0;
        }
    }

    public List<String> keys(Item item) {
        int count = api().itemDataCount(mContext.handle(), item.handle());
        List<String> keys = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            keys.add(Interop.string(api().itemDataKey(mContext.handle(), item.handle(), index)));
        }
        return keys;
    }

    private static Optional<String> optional(MemorySegment value) {
        return Interop.isNull(value) ? Optional.empty() : Optional.of(Interop.string(value));
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
