package falcon.api;

/**
 * A skin texture: RGBA, four bytes a pixel, rows from the top.
 */
public record SkinImage(int width, int height, byte[] rgba) {
}
