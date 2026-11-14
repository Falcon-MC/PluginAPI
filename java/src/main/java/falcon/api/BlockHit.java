package falcon.api;

/**
 * The block a ray hit; position is where the ray enters it.
 */
public record BlockHit(BlockPos block, BlockFace face, Vec3 position, double distance) {
}
