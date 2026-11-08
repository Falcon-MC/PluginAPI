using System;
using Falcon.Interop;

namespace Falcon
{
    /// <summary>
    /// How an image meets a grid of another aspect ratio. Contain shows the whole image with transparent margins,
    /// Cover fills the grid and crops the overflow, Stretch distorts it.
    /// </summary>
    public enum ImageFit : uint
    {
        Stretch = FalconConstants.ImageFitStretch,
        Contain = FalconConstants.ImageFitContain,
        Cover = FalconConstants.ImageFitCover
    }

    /// <summary>
    /// The way the front of the frames points, the facing_direction of an item frame.
    /// </summary>
    public enum WallFacing : uint
    {
        North = 2,
        South = 3,
        West = 4,
        East = 5
    }

    /// <summary>
    /// Map ids row by row from the top left; empty when the grid could not be made.
    /// </summary>
    public sealed class ImageMapGrid
    {
        public ImageMapGrid(uint columns, uint rows, long[] ids)
        {
            Columns = columns;
            Rows = rows;
            Ids = ids;
        }

        public uint Columns { get; }

        public uint Rows { get; }

        public long[] Ids { get; }

        public bool IsEmpty
        {
            get
            {
                return Ids.Length == 0;
            }
        }
    }

    /// <summary>
    /// Pictures on locked vanilla maps, 128 x 128 pixels each. Pixels are rgba bytes, rows top to bottom. The maps are
    /// saved with the world, never drawn over by the world and sent to players like any other map. A grid goes up to
    /// 32 x 32 maps and images to 4096 pixels a side. On failure LastError says why.
    /// </summary>
    public static unsafe class ImageMaps
    {
        public const uint MapSize = FalconConstants.MapSize;

        private const int MaxMaps = 32 * 32;

        public static string LastError
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->mapLastError());
            }
        }

        /// <summary>
        /// One map from exactly 128 * 128 * 4 bytes; -1 on failure.
        /// </summary>
        public static long Create(Level level, ReadOnlySpan<byte> rgba)
        {
            fixed (byte* bytes = rgba)
            {
                return NativeApi.Table->mapCreateImage(level.Raw, bytes, (uint)rgba.Length);
            }
        }

        /// <summary>
        /// Cuts a width x height image into a grid. A side of 0 is picked from the image: both 0 keep its size, one 0
        /// keeps its aspect ratio.
        /// </summary>
        public static ImageMapGrid Create(Level level, ReadOnlySpan<byte> rgba, uint width, uint height,
            uint columns = 0, uint rows = 0, ImageFit fit = ImageFit.Contain)
        {
            long[] ids = new long[MaxMaps];
            uint gridColumns = 0;
            uint gridRows = 0;
            uint count;
            fixed (byte* bytes = rgba)
            fixed (long* output = ids)
            {
                count = NativeApi.Table->mapCreateImageGrid(level.Raw, bytes, (uint)rgba.Length, width, height, columns,
                    rows, (uint)fit, output, (uint)ids.Length, &gridColumns, &gridRows);
            }

            return new ImageMapGrid(gridColumns, gridRows, ids.AsSpan(0, (int)count).ToArray());
        }

        /// <summary>
        /// The same from the bytes of a png file, at most 32 MB.
        /// </summary>
        public static ImageMapGrid CreateFromPng(Level level, ReadOnlySpan<byte> png, uint columns = 0, uint rows = 0,
            ImageFit fit = ImageFit.Contain)
        {
            long[] ids = new long[MaxMaps];
            uint gridColumns = 0;
            uint gridRows = 0;
            uint count;
            fixed (byte* bytes = png)
            fixed (long* output = ids)
            {
                count = NativeApi.Table->mapCreateImageGridFromPng(level.Raw, bytes, (uint)png.Length, columns, rows,
                    (uint)fit, output, (uint)ids.Length, &gridColumns, &gridRows);
            }

            return new ImageMapGrid(gridColumns, gridRows, ids.AsSpan(0, (int)count).ToArray());
        }

        /// <summary>
        /// Hangs the grid in item frames, topLeft being the frame of the first map. Each spot must be air with a solid
        /// block behind it, or a frame with this facing that is empty or holds a picture. Nothing changes on failure.
        /// </summary>
        public static bool Place(Level level, BlockPos topLeft, WallFacing facing, ImageMapGrid grid,
            bool glowing = false)
        {
            fixed (long* ids = grid.Ids)
            {
                return NativeApi.Table->mapPlaceImageGrid(level.Raw, topLeft.ToNative(), (uint)facing, ids,
                    grid.Columns, grid.Rows, glowing ? 1 : 0) != 0;
            }
        }

        /// <summary>
        /// Replaces a width x height block of pixels; only the pixels that changed go to the players who see it.
        /// </summary>
        public static bool Update(Level level, long mapId, uint x, uint y, uint width, uint height,
            ReadOnlySpan<byte> rgba)
        {
            fixed (byte* bytes = rgba)
            {
                return NativeApi.Table->mapUpdateImage(level.Raw, mapId, x, y, width, height, bytes,
                    (uint)rgba.Length) != 0;
            }
        }

        public static bool IsImageMap(Level level, long mapId)
        {
            return NativeApi.Table->mapIsImage(level.Raw, mapId) != 0;
        }
    }
}
