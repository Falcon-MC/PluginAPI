package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Entity;
import falcon.api.Item;
import falcon.api.Level;
import falcon.api.Player;
import falcon.api.Vec3;
import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.Optional;

public abstract class Event {
    final PluginContext mContext;
    final MemorySegment mHandle;

    Event(PluginContext context, MemorySegment handle) {
        mContext = context;
        mHandle = handle;
    }

    public boolean isCancelled() {
        return api().eventIsCancelled(mHandle) != 0;
    }

    public void setCancelled(boolean cancelled) {
        api().eventSetCancelled(mHandle, cancelled ? 1 : 0);
    }

    public void cancel() {
        setCancelled(true);
    }

    public MemorySegment handle() {
        return mHandle;
    }

    static NativeApi api() {
        return Interop.api();
    }

    Player eventPlayer() {
        return new Player(mContext, api().eventPlayer(mHandle));
    }

    Entity eventEntity() {
        return new Entity(mContext, api().eventEntity(mHandle));
    }

    Optional<Entity> optionalEntity(MemorySegment entity) {
        if (Interop.isNull(entity)) {
            return Optional.empty();
        }
        return Optional.of(new Entity(mContext, entity));
    }

    Level eventLevel() {
        return new Level(mContext, api().eventLevel(mHandle));
    }

    Item eventItem() {
        return Item.borrow(api().eventItem(mHandle));
    }

    BlockPos eventBlockPosition() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.blockPos(api().eventBlockPosition(arena, mHandle));
        }
    }

    String eventBlockName() {
        return Interop.string(api().eventBlockName(mHandle));
    }

    String eventMessage() {
        return Interop.string(api().eventMessage(mHandle));
    }

    void eventSetMessage(String message) {
        try (Arena arena = Arena.ofConfined()) {
            api().eventSetMessage(mHandle, Interop.text(arena, message));
        }
    }

    String eventCause() {
        return Interop.string(api().eventCause(mHandle));
    }

    Vec3 eventFrom() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().eventFrom(arena, mHandle));
        }
    }

    Vec3 eventTo() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().eventTo(arena, mHandle));
        }
    }

    void eventSetTo(Vec3 position) {
        try (Arena arena = Arena.ofConfined()) {
            api().eventSetTo(mHandle, Interop.vec3(arena, position));
        }
    }

    Vec3 eventPosition() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().eventPosition(arena, mHandle));
        }
    }
}
