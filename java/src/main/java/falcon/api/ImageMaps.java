package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

/**
 * Pictures on locked vanilla maps, 128 x 128 pixels each. Pixels are rgba bytes, rows top to bottom. The maps are
 * saved with the world, never drawn over by the world and sent to players like any other map. A grid goes up to
 * 32 x 32 maps and images to 4096 pixels a side. On failure {@link #lastError()} says why.
 */
public final class ImageMaps {
    public static final int MAP_SIZE = FalconAbi.MAP_SIZE;

    private static final int MAX_MAPS = 32 * 32;

    private ImageMaps() {
    }

    /** One map from exactly 128 * 128 * 4 bytes; -1 on failure. */
    public static long create(Level level, byte[] rgba) {
        try (Arena arena = Arena.ofConfined()) {
            return api().mapCreateImage(level.handle(), bytes(arena, rgba), rgba.length);
        }
    }

    public static ImageMapGrid create(Level level, byte[] rgba, int width, int height) {
        return create(level, rgba, width, height, 0, 0, ImageFit.CONTAIN);
    }

    /**
     * Cuts a width x height image into a grid. A side of 0 is picked from the image: both 0 keep its size, one 0
     * keeps its aspect ratio.
     */
    public static ImageMapGrid create(Level level, byte[] rgba, int width, int height, int columns, int rows,
                                      ImageFit fit) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment ids = arena.allocate(ValueLayout.JAVA_LONG, MAX_MAPS);
            MemorySegment gridColumns = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment gridRows = arena.allocate(ValueLayout.JAVA_INT);
            int count = api().mapCreateImageGrid(level.handle(), bytes(arena, rgba), rgba.length, width, height,
                    columns, rows, fit.value(), ids, MAX_MAPS, gridColumns, gridRows);
            return grid(ids, count, gridColumns, gridRows);
        }
    }

    public static ImageMapGrid createFromPng(Level level, byte[] png) {
        return createFromPng(level, png, 0, 0, ImageFit.CONTAIN);
    }

    /** The same from the bytes of a png file, at most 32 MB. */
    public static ImageMapGrid createFromPng(Level level, byte[] png, int columns, int rows, ImageFit fit) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment ids = arena.allocate(ValueLayout.JAVA_LONG, MAX_MAPS);
            MemorySegment gridColumns = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment gridRows = arena.allocate(ValueLayout.JAVA_INT);
            int count = api().mapCreateImageGridFromPng(level.handle(), bytes(arena, png), png.length, columns, rows,
                    fit.value(), ids, MAX_MAPS, gridColumns, gridRows);
            return grid(ids, count, gridColumns, gridRows);
        }
    }

    /**
     * Hangs the grid in item frames, topLeft being the frame of the first map. Each spot must be air with a solid
     * block behind it, or a frame with this facing that is empty or holds a picture. Nothing changes on failure.
     */
    public static boolean place(Level level, BlockPos topLeft, WallFacing facing, ImageMapGrid grid,
                                boolean glowing) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment ids = grid.ids().length == 0 ? MemorySegment.NULL
                    : arena.allocateFrom(ValueLayout.JAVA_LONG, grid.ids());
            return api().mapPlaceImageGrid(level.handle(), Interop.blockPos(arena, topLeft), facing.value(), ids,
                    grid.columns(), grid.rows(), glowing ? 1 : 0) != 0;
        }
    }

    /** Replaces a width x height block of pixels; only the pixels that changed go to the players who see it. */
    public static boolean update(Level level, long mapId, int x, int y, int width, int height, byte[] rgba) {
        try (Arena arena = Arena.ofConfined()) {
            return api().mapUpdateImage(level.handle(), mapId, x, y, width, height, bytes(arena, rgba),
                    rgba.length) != 0;
        }
    }

    public static boolean isImageMap(Level level, long mapId) {
        return api().mapIsImage(level.handle(), mapId) != 0;
    }

    public static String lastError() {
        return Interop.string(api().mapLastError());
    }

    private static MemorySegment bytes(Arena arena, byte[] data) {
        return data.length == 0 ? MemorySegment.NULL : arena.allocateFrom(ValueLayout.JAVA_BYTE, data);
    }

    private static ImageMapGrid grid(MemorySegment ids, int count, MemorySegment columns, MemorySegment rows) {
        long[] result = ids.asSlice(0, (long) count * Long.BYTES).toArray(ValueLayout.JAVA_LONG);
        return new ImageMapGrid(columns.get(ValueLayout.JAVA_INT, 0), rows.get(ValueLayout.JAVA_INT, 0), result);
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
