#pragma once

#include "falcon/Api.hpp"
#include "falcon/BlockPos.hpp"
#include "falcon/Level.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace falcon {
    /**
     * How an image meets a grid of another aspect ratio. Contain shows the whole image with transparent margins,
     * Cover fills the grid and crops the overflow, Stretch distorts it.
     */
    enum class ImageFit : uint32_t {
        Stretch = FALCON_IMAGE_FIT_STRETCH,
        Contain = FALCON_IMAGE_FIT_CONTAIN,
        Cover = FALCON_IMAGE_FIT_COVER
    };

    /** The way the front of the frames points, the facing_direction of an item frame. */
    enum class WallFacing : uint32_t {
        North = 2,
        South = 3,
        West = 4,
        East = 5
    };

    /** Map ids row by row from the top left; empty when the grid could not be made. */
    struct ImageMapGrid {
        uint32_t columns = 0;
        uint32_t rows = 0;
        std::vector<int64_t> ids;

        bool empty() const {
            return ids.empty();
        }
    };

    /**
     * Pictures on locked vanilla maps, 128 x 128 pixels each. Pixels are rgba bytes, rows top to bottom. The maps
     * are saved with the world, never drawn over by the world and sent to players like any other map. A grid goes up
     * to 32 x 32 maps and images to 4096 pixels a side. On failure lastError() says why.
     */
    class ImageMaps {
    public:
        static constexpr uint32_t MAP_SIZE = FALCON_MAP_SIZE;

        /** One map from exactly 128 * 128 * 4 bytes; -1 on failure. */
        static int64_t create(const Level &level, const std::vector<uint8_t> &rgba) {
            return detail::api().mapCreateImage(level.handle(), rgba.data(), static_cast<uint32_t>(rgba.size()));
        }

        /**
         * Cuts a width x height image into a grid. A side of 0 is picked from the image: both 0 keep its size, one 0
         * keeps its aspect ratio.
         */
        static ImageMapGrid create(const Level &level, const std::vector<uint8_t> &rgba, uint32_t width,
                                   uint32_t height, uint32_t columns = 0, uint32_t rows = 0,
                                   ImageFit fit = ImageFit::Contain) {
            ImageMapGrid grid;
            std::vector<int64_t> ids(maxMaps());
            const uint32_t count = detail::api().mapCreateImageGrid(
                    level.handle(), rgba.data(), static_cast<uint32_t>(rgba.size()), width, height, columns, rows,
                    static_cast<uint32_t>(fit), ids.data(), static_cast<uint32_t>(ids.size()), &grid.columns,
                    &grid.rows);
            ids.resize(count);
            grid.ids = std::move(ids);
            return grid;
        }

        /** The same from the bytes of a png file, at most 32 MB. */
        static ImageMapGrid createFromPng(const Level &level, const std::vector<uint8_t> &png, uint32_t columns = 0,
                                          uint32_t rows = 0, ImageFit fit = ImageFit::Contain) {
            ImageMapGrid grid;
            std::vector<int64_t> ids(maxMaps());
            const uint32_t count = detail::api().mapCreateImageGridFromPng(
                    level.handle(), png.data(), static_cast<uint32_t>(png.size()), columns, rows,
                    static_cast<uint32_t>(fit), ids.data(), static_cast<uint32_t>(ids.size()), &grid.columns,
                    &grid.rows);
            ids.resize(count);
            grid.ids = std::move(ids);
            return grid;
        }

        /**
         * Hangs the grid in item frames, topLeft being the frame of the first map. Each spot must be air with a solid
         * block behind it, or a frame with this facing that is empty or holds a picture. The grid needs exactly
         * columns * rows ids. Nothing changes on failure.
         */
        static bool place(const Level &level, const BlockPos &topLeft, WallFacing facing, const ImageMapGrid &grid,
                          bool glowing = false) {
            return detail::api().mapPlaceImageGrid(level.handle(), topLeft.raw(), static_cast<uint32_t>(facing),
                                                   grid.ids.data(), static_cast<uint32_t>(grid.ids.size()),
                                                   grid.columns, grid.rows, glowing ? 1 : 0) != 0;
        }

        /** Replaces a width x height block of pixels; only the pixels that changed go to the players who see it. */
        static bool update(const Level &level, int64_t mapId, uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height, const std::vector<uint8_t> &rgba) {
            return detail::api().mapUpdateImage(level.handle(), mapId, x, y, width, height, rgba.data(),
                                                static_cast<uint32_t>(rgba.size())) != 0;
        }

        static bool isImageMap(const Level &level, int64_t mapId) {
            return detail::api().mapIsImage(level.handle(), mapId) != 0;
        }

        /**
         * Erases an image map from memory and from the world save, and takes it out of the frames of loaded chunks
         * and the inventories of online players. Copies anywhere else stay as blank maps.
         */
        static bool remove(const Level &level, int64_t mapId) {
            return detail::api().mapDeleteImage(level.handle(), mapId) != 0;
        }

        static std::string lastError() {
            return detail::text(detail::api().mapLastError());
        }

    private:
        static size_t maxMaps() {
            return 32 * 32;
        }
    };
}
