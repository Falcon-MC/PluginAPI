#pragma once

#include "falcon/falcon_api.h"

namespace falcon {
    struct Vec3 {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        Vec3() = default;

        Vec3(double valueX, double valueY, double valueZ) : x(valueX), y(valueY), z(valueZ) {
        }

        static Vec3 from(const FalconVec3 &value) {
            return Vec3(value.x, value.y, value.z);
        }

        FalconVec3 raw() const {
            return FalconVec3{x, y, z};
        }

        Vec3 operator+(const Vec3 &other) const {
            return Vec3(x + other.x, y + other.y, z + other.z);
        }

        Vec3 operator-(const Vec3 &other) const {
            return Vec3(x - other.x, y - other.y, z - other.z);
        }

        Vec3 operator*(double factor) const {
            return Vec3(x * factor, y * factor, z * factor);
        }

        bool operator==(const Vec3 &other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Vec3 &other) const {
            return !(*this == other);
        }
    };
}
