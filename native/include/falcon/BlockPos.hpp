#pragma once

#include "falcon/Vec3.hpp"
#include "falcon/falcon_api.h"

#include <cmath>
#include <cstdint>

namespace falcon {
    struct BlockPos {
        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;

        BlockPos() = default;

        BlockPos(int32_t valueX, int32_t valueY, int32_t valueZ) : x(valueX), y(valueY), z(valueZ) {
        }

        static BlockPos from(const FalconBlockPos &value) {
            return BlockPos(value.x, value.y, value.z);
        }

        static BlockPos floor(const Vec3 &position) {
            return BlockPos(static_cast<int32_t>(std::floor(position.x)), static_cast<int32_t>(std::floor(position.y)),
                            static_cast<int32_t>(std::floor(position.z)));
        }

        FalconBlockPos raw() const {
            return FalconBlockPos{x, y, z};
        }

        Vec3 center() const {
            return Vec3(x + 0.5, y + 0.5, z + 0.5);
        }

        int32_t chunkX() const {
            return x >> 4;
        }

        int32_t chunkZ() const {
            return z >> 4;
        }

        BlockPos offset(int32_t deltaX, int32_t deltaY, int32_t deltaZ) const {
            return BlockPos(x + deltaX, y + deltaY, z + deltaZ);
        }

        bool operator==(const BlockPos &other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const BlockPos &other) const {
            return !(*this == other);
        }
    };
}
