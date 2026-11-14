package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.util.ArrayList;
import java.util.List;
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

    public boolean isInvisible() {
        return api().entityIsInvisible(mHandle) != 0;
    }

    /**
     * Hides the entity from every viewer, its name tag included, until it is made visible again.
     */
    public void setInvisible(boolean invisible) {
        api().entitySetInvisible(mHandle, invisible ? 1 : 0);
    }

    public void extinguish() {
        setOnFire(0);
    }

    public boolean addEffect(EffectType type, int duration) {
        return addEffect(type, duration, 0, true);
    }

    /**
     * Adds a potion effect for duration ticks, or {@link Effect#INFINITE}. A weaker or shorter effect than the one
     * running is refused, as in the game.
     */
    public boolean addEffect(EffectType type, int duration, int amplifier, boolean showParticles) {
        return api().entityAddEffect(mHandle, type.value(), duration, amplifier, showParticles ? 1 : 0) != 0;
    }

    public boolean removeEffect(EffectType type) {
        return api().entityRemoveEffect(mHandle, type.value()) != 0;
    }

    public Optional<Effect> effect(EffectType type) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment out = arena.allocate(FalconAbi.Effect.LAYOUT);
            if (api().entityEffect(mHandle, type.value(), out) == 0) {
                return Optional.empty();
            }
            return Optional.of(Effect.read(out));
        }
    }

    public boolean hasEffect(EffectType type) {
        return effect(type).isPresent();
    }

    public List<Effect> effects() {
        int count = api().entityEffectCount(mHandle);
        List<Effect> result = new ArrayList<>(Math.max(count, 0));
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment out = arena.allocate(FalconAbi.Effect.LAYOUT);
            for (int index = 0; index < count; index++) {
                if (api().entityEffectAt(mHandle, index, out) != 0) {
                    result.add(Effect.read(out));
                }
            }
        }
        return result;
    }

    public void clearEffects() {
        api().entityClearEffects(mHandle);
    }

    /**
     * An attribute by its identifier, such as minecraft:movement or minecraft:health. Only the attributes the entity
     * has are found: {@link #attributes()} lists them.
     */
    public Optional<Attribute> attribute(String name) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment out = arena.allocate(FalconAbi.Attribute.LAYOUT);
            if (api().entityAttribute(mHandle, Interop.text(arena, name), out) == 0) {
                return Optional.empty();
            }
            return Optional.of(Attribute.read(name, out));
        }
    }

    public List<Attribute> attributes() {
        int count = api().entityAttributeCount(mHandle);
        List<Attribute> result = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            attribute(Interop.string(api().entityAttributeName(mHandle, index))).ifPresent(result::add);
        }
        return result;
    }

    /**
     * Returns false when the entity has no such attribute or the value is outside its range.
     */
    public boolean setAttribute(String name, float value) {
        try (Arena arena = Arena.ofConfined()) {
            return api().entitySetAttribute(mHandle, Interop.text(arena, name), value) != 0;
        }
    }

    /**
     * What the server would save for this entity, as little-endian NBT; read only.
     */
    public byte[] nbt() {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment length = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment data = api().entityNbt(mHandle, length);
            int size = length.get(ValueLayout.JAVA_INT, 0);
            if (Interop.isNull(data) || size <= 0) {
                return new byte[0];
            }
            return data.reinterpret(size).toArray(ValueLayout.JAVA_BYTE);
        }
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
