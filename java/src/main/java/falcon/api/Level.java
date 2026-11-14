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

    public List<String> gameRules() {
        int count = api().levelGameRuleCount(mHandle);
        List<String> names = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().levelGameRuleName(mHandle, index)));
        }
        return names;
    }

    /**
     * "true" or "false" for a switch, the number otherwise; the rules belong to the level's world.
     */
    public Optional<String> gameRule(String name) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment value = api().levelGameRule(mHandle, Interop.text(arena, name));
            return Interop.isNull(value) ? Optional.empty() : Optional.of(Interop.string(value));
        }
    }

    public boolean setGameRule(String name, String value) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelSetGameRule(mHandle, Interop.text(arena, name), Interop.text(arena, value)) != 0;
        }
    }

    /**
     * The biome identifier at a position, such as minecraft:plains.
     */
    public String biome(BlockPos position) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.string(api().levelBiome(mHandle, Interop.blockPos(arena, position)));
        }
    }

    /**
     * The first block on the ray; an unloaded chunk stops it without a hit.
     */
    public Optional<BlockHit> raycastBlock(Vec3 origin, Vec3 direction, double maxDistance, boolean includeLiquids,
                                           boolean includePassable) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment hit = arena.allocate(FalconAbi.BlockHit.LAYOUT);
            if (api().levelRaycastBlock(mHandle, Interop.vec3(arena, origin), Interop.vec3(arena, direction),
                    maxDistance, includeLiquids ? 1 : 0, includePassable ? 1 : 0, hit) == 0) {
                return Optional.empty();
            }
            BlockPos block = Interop.blockPos(hit.asSlice(FalconAbi.BlockHit.BLOCK, FalconAbi.BlockPos.LAYOUT));
            Vec3 position = Interop.vec3(hit.asSlice(FalconAbi.BlockHit.POSITION, FalconAbi.Vec3.LAYOUT));
            return Optional.of(new BlockHit(block, BlockFace.of(hit.get(ValueLayout.JAVA_INT, FalconAbi.BlockHit.FACE)),
                    position, hit.get(ValueLayout.JAVA_DOUBLE, FalconAbi.BlockHit.DISTANCE)));
        }
    }

    /**
     * The nearest entity on the ray, before any block; except, usually the one looking, may be null.
     */
    public Optional<EntityHit> raycastEntity(Vec3 origin, Vec3 direction, double maxDistance, Entity except) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment distance = arena.allocate(ValueLayout.JAVA_DOUBLE);
            MemorySegment hit = api().levelRaycastEntity(mHandle, Interop.vec3(arena, origin),
                    Interop.vec3(arena, direction), maxDistance, except == null ? MemorySegment.NULL : except.handle(),
                    distance);
            if (Interop.isNull(hit)) {
                return Optional.empty();
            }
            return Optional.of(new EntityHit(new Entity(mContext, hit), distance.get(ValueLayout.JAVA_DOUBLE, 0)));
        }
    }

    /**
     * Every entity and player whose box touches the box between two corners.
     */
    public List<Entity> entitiesInBox(Vec3 from, Vec3 to) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment low = Interop.vec3(arena, from);
            MemorySegment high = Interop.vec3(arena, to);
            int capacity = 64;
            MemorySegment handles = arena.allocate(ValueLayout.ADDRESS, capacity);
            int count = api().levelEntitiesInBox(mHandle, low, high, handles, capacity);
            if (count > capacity) {
                capacity = count;
                handles = arena.allocate(ValueLayout.ADDRESS, capacity);
                count = api().levelEntitiesInBox(mHandle, low, high, handles, capacity);
            }
            List<Entity> result = new ArrayList<>(Math.max(count, 0));
            for (int index = 0; index < Math.min(count, capacity); index++) {
                result.add(new Entity(mContext, handles.getAtIndex(ValueLayout.ADDRESS, index)));
            }
            return result;
        }
    }

    public boolean placeStructure(String structure, BlockPos position) {
        return placeStructure(structure, position, 0, Mirror.NONE, true, 1.0f);
    }

    /**
     * Places a saved structure by identifier with its lowest corner at position. Rotation is in quarter turns
     * clockwise; integrity keeps that share of blocks.
     */
    public boolean placeStructure(String structure, BlockPos position, int rotation, Mirror mirror,
                                  boolean includeEntities, float integrity) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelPlaceStructure(mHandle, Interop.text(arena, structure), Interop.blockPos(arena, position),
                    rotation, mirror.value(), includeEntities ? 1 : 0, integrity) != 0;
        }
    }

    /**
     * Keeps the chunks between two corners loaded and ticking, with the limits of /tickingarea.
     */
    public boolean addTickingArea(String name, BlockPos from, BlockPos to, boolean preload) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelAddTickingArea(mHandle, Interop.text(arena, name), Interop.blockPos(arena, from),
                    Interop.blockPos(arena, to), preload ? 1 : 0) != 0;
        }
    }

    public boolean removeTickingArea(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().levelRemoveTickingArea(mHandle, Interop.text(arena, name)) != 0;
        }
    }

    public List<String> tickingAreas() {
        int count = api().levelTickingAreaCount(mHandle);
        List<String> names = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().levelTickingAreaName(mHandle, index)));
        }
        return names;
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
