#pragma once

#include "falcon/BlockPos.hpp"
#include "falcon/Player.hpp"

#include <cstdint>
#include <functional>
#include <string>

namespace falcon {
    struct CustomBlock {
        std::string identifier;
        std::string displayName;
        std::string texture;
        std::string creativeCategory;
        float destroyTime = 1.0f;
        float explosionResistance = 1.0f;
        uint32_t lightEmission = 0;
        float friction = 0.6f;
        std::string drop;
        std::function<bool(Player &, const BlockPos &, uint32_t)> onInteract;
        std::function<void(Player &, const BlockPos &)> onBreak;
    };
}
