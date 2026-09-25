package falcon.api;

public record BlockPos(int x, int y, int z) {
    public static BlockPos floor(Vec3 position) {
        return new BlockPos((int) Math.floor(position.x()), (int) Math.floor(position.y()),
                (int) Math.floor(position.z()));
    }

    public Vec3 center() {
        return new Vec3(x + 0.5, y + 0.5, z + 0.5);
    }

    public int chunkX() {
        return x >> 4;
    }

    public int chunkZ() {
        return z >> 4;
    }

    public BlockPos offset(int deltaX, int deltaY, int deltaZ) {
        return new BlockPos(x + deltaX, y + deltaY, z + deltaZ);
    }
}
