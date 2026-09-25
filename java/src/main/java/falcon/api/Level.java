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
