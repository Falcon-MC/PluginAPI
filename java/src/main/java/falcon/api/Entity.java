package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.Optional;

public final class Entity {
    private final PluginContext mContext;
    private final MemorySegment mHandle;

    public Entity(PluginContext context, MemorySegment handle) {
        mContext = context;
        mHandle = handle == null ? MemorySegment.NULL : handle;
    }

    public boolean valid() {
        return !Interop.isNull(mHandle);
    }

    public String type() {
        return Interop.string(api().entityType(mHandle));
    }

    public long runtimeId() {
        return api().entityRuntimeId(mHandle);
    }

    public Level level() {
        return new Level(mContext, api().entityLevel(mHandle));
    }

    public Vec3 position() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().entityPosition(arena, mHandle));
        }
    }

    public Vec3 rotation() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().entityRotation(arena, mHandle));
        }
    }

    public void teleport(Vec3 position) {
        try (Arena arena = Arena.ofConfined()) {
            api().entityTeleport(mHandle, MemorySegment.NULL, Interop.vec3(arena, position));
        }
    }

    public void teleport(Vec3 position, Level level) {
        try (Arena arena = Arena.ofConfined()) {
            api().entityTeleport(mHandle, level.handle(), Interop.vec3(arena, position));
        }
    }

    public Vec3 motion() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().entityMotion(arena, mHandle));
        }
    }

    public void setMotion(Vec3 motion) {
        try (Arena arena = Arena.ofConfined()) {
            api().entitySetMotion(mHandle, Interop.vec3(arena, motion));
        }
    }

    public float health() {
        return api().entityHealth(mHandle);
    }

    public float maxHealth() {
        return api().entityMaxHealth(mHandle);
    }

    public void setHealth(float health) {
        api().entitySetHealth(mHandle, health);
    }

    public boolean isAlive() {
        return api().entityIsAlive(mHandle) != 0;
    }

    public boolean damage(float amount) {
        return damage(amount, "");
    }

    public boolean damage(float amount, String cause) {
        try (Arena arena = Arena.ofConfined()) {
            return api().entityDamage(mHandle, amount, Interop.nullableText(arena, cause), MemorySegment.NULL) != 0;
        }
    }

    public boolean damage(float amount, String cause, Entity attacker) {
        try (Arena arena = Arena.ofConfined()) {
            return api().entityDamage(mHandle, amount, Interop.nullableText(arena, cause), attacker.mHandle) != 0;
        }
    }

    public void kill() {
        api().entityKill(mHandle);
    }

    public void remove() {
        api().entityRemove(mHandle);
    }

    public String nameTag() {
        return Interop.string(api().entityNameTag(mHandle));
    }

    public void setNameTag(String nameTag) {
        try (Arena arena = Arena.ofConfined()) {
            api().entitySetNameTag(mHandle, Interop.text(arena, nameTag));
        }
    }

    public boolean isOnFire() {
        return api().entityIsOnFire(mHandle) != 0;
    }

    public void setOnFire(int ticks) {
        api().entitySetOnFire(mHandle, ticks);
    }

    public void extinguish() {
        setOnFire(0);
    }

    public Optional<Player> asPlayer() {
        MemorySegment player = api().entityPlayer(mHandle);
        if (Interop.isNull(player)) {
            return Optional.empty();
        }
        return Optional.of(new Player(mContext, player));
    }

    public MemorySegment handle() {
        return mHandle;
    }

    @Override
    public boolean equals(Object other) {
        return other instanceof Entity entity && entity.mHandle.address() == mHandle.address();
    }

    @Override
    public int hashCode() {
        return Long.hashCode(mHandle.address());
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
