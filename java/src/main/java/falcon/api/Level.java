package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public final class Level {
    private final PluginContext mContext;
    private final MemorySegment mHandle;

    public Level(PluginContext context, MemorySegment handle) {
        mContext = context;
        mHandle = handle == null ? MemorySegment.NULL : handle;
    }

    public boolean valid() {
        return !Interop.isNull(mHandle);
    }

    public Dimension dimension() {
        return Dimension.of(api().levelDimension(mHandle));
    }

    public String name() {
        return Interop.string(api().levelName(mHandle));
    }

    public Block getBlock(BlockPos position) {
        return new Block(position, blockName(position), blockStates(position));
    }

    public String blockName(BlockPos position) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.string(api().levelGetBlock(mHandle, Interop.blockPos(arena, position)));
        }
    }

    public String blockStates(BlockPos position) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.string(api().levelGetBlockStates(mHandle, Interop.blockPos(arena, position)));
        }
    }

    public boolean setBlock(BlockPos position, String name) {
        return setBlock(position, name, "");
    }

    public boolean setBlock(BlockPos position, String name, String statesJson) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment target = Interop.blockPos(arena, position);
            MemorySegment states = Interop.nullableText(arena, statesJson);
            return api().levelSetBlock(mHandle, target, Interop.text(arena, name), states) != 0;
        }
    }

    public boolean breakBlock(BlockPos position) {
        return breakBlock(position, true);
    }

    public boolean breakBlock(BlockPos position, boolean dropItems) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelBreakBlock(mHandle, Interop.blockPos(arena, position), dropItems ? 1 : 0) != 0;
        }
    }

    public boolean isChunkLoaded(int chunkX, int chunkZ) {
        return api().levelIsChunkLoaded(mHandle, chunkX, chunkZ) != 0;
    }

    public int highestBlockY(int x, int z) {
        return api().levelHighestBlockY(mHandle, x, z);
    }

    public long time() {
        return api().levelTime(mHandle);
    }

    public void setTime(long time) {
        api().levelSetTime(mHandle, time);
    }

    public boolean isRaining() {
        return api().levelIsRaining(mHandle) != 0;
    }

    public void setRaining(boolean raining) {
        api().levelSetRaining(mHandle, raining ? 1 : 0);
    }

    public boolean isThundering() {
        return api().levelIsThundering(mHandle) != 0;
    }

    public void setThundering(boolean thundering) {
        api().levelSetThundering(mHandle, thundering ? 1 : 0);
    }

    public Vec3 spawnPosition() {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.vec3(api().levelSpawnPosition(arena, mHandle));
        }
    }

    public Optional<Entity> spawnEntity(String identifier, Vec3 position) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment target = Interop.vec3(arena, position);
            MemorySegment entity = api().levelSpawnEntity(mHandle, Interop.text(arena, identifier), target);
            if (Interop.isNull(entity)) {
                return Optional.empty();
            }
            return Optional.of(new Entity(mContext, entity));
        }
    }

    public void dropItem(Vec3 position, Item item) {
        try (Arena arena = Arena.ofConfined()) {
            api().levelDropItem(mHandle, Interop.vec3(arena, position), item.handle());
        }
    }

    public void strikeLightning(Vec3 position) {
        try (Arena arena = Arena.ofConfined()) {
            api().levelStrikeLightning(mHandle, Interop.vec3(arena, position));
        }
    }

    public void createExplosion(Vec3 position, float power) {
        createExplosion(position, power, true);
    }

    public void createExplosion(Vec3 position, float power, boolean breakBlocks) {
        try (Arena arena = Arena.ofConfined()) {
            api().levelCreateExplosion(mHandle, Interop.vec3(arena, position), power, breakBlocks ? 1 : 0);
        }
    }

    public void playSound(String sound, Vec3 position) {
        playSound(sound, position, 1.0f, 1.0f);
    }

    /**
     * Plays a sound for every player in this level; the client fades it with distance.
     */
    public void playSound(String sound, Vec3 position, float volume, float pitch) {
        try (Arena arena = Arena.ofConfined()) {
            api().levelPlaySound(mHandle, Interop.text(arena, sound), Interop.vec3(arena, position), volume, pitch);
        }
    }

    public void spawnParticle(String particle, Vec3 position) {
        try (Arena arena = Arena.ofConfined()) {
            api().levelSpawnParticle(mHandle, Interop.text(arena, particle), Interop.vec3(arena, position));
        }
    }

    /**
     * The slot count of the chest, barrel, furnace, hopper or other container block there; 0 for none.
     */
    public int containerSize(BlockPos position) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelContainerSize(mHandle, Interop.blockPos(arena, position));
        }
    }

    /**
     * A copy of the item in a container slot; empty for an empty slot or no container.
     */
    public Optional<Item> containerItem(BlockPos position, int slot) {
        try (Arena arena = Arena.ofConfined()) {
            return Item.adoptFilled(api().levelContainerItem(mHandle, Interop.blockPos(arena, position), slot));
        }
    }

    public boolean setContainerItem(BlockPos position, int slot, Item item) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelSetContainerItem(mHandle, Interop.blockPos(arena, position), slot, item.handle()) != 0;
        }
    }

    public boolean clearContainerItem(BlockPos position, int slot) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelSetContainerItem(mHandle, Interop.blockPos(arena, position), slot,
                    MemorySegment.NULL) != 0;
        }
    }

    public Optional<String> signText(BlockPos position) {
        return signText(position, SignSide.FRONT);
    }

    /**
     * The text of a sign or hanging sign, raw or JSON as the game saved it; empty when no sign is there.
     */
    public Optional<String> signText(BlockPos position, SignSide side) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment text = api().levelSignText(mHandle, Interop.blockPos(arena, position), side.value());
            return Interop.isNull(text) ? Optional.empty() : Optional.of(Interop.string(text));
        }
    }

    public boolean setSignText(BlockPos position, String text) {
        return setSignText(position, text, SignSide.FRONT);
    }

    public boolean setSignText(BlockPos position, String text, SignSide side) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelSetSignText(mHandle, Interop.blockPos(arena, position), side.value(),
                    Interop.text(arena, text)) != 0;
        }
    }

    public List<Entity> entities() {
        int count = api().levelEntityCount(mHandle);
        List<Entity> result = new ArrayList<>(count);
        for (int index = 0; index < count; index++) {
            result.add(new Entity(mContext, api().levelEntity(mHandle, index)));
        }
        return result;
    }

    public MemorySegment handle() {
        return mHandle;
    }

    @Override
    public boolean equals(Object other) {
        return other instanceof Level level && level.mHandle.address() == mHandle.address();
    }

    @Override
    public int hashCode() {
        return Long.hashCode(mHandle.address());
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
