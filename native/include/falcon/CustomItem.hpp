#pragma once

#include "falcon/BlockPos.hpp"
#include "falcon/Item.hpp"
#include "falcon/Player.hpp"

#include <cstdint>
#include <functional>
#include <string>

namespace falcon {
    struct CustomItem {
        std::string identifier;
        std::string displayName;
        std::string icon;
        std::string creativeCategory;
        uint32_t maxStackSize = 64;
        uint32_t maxDurability = 0;
        bool handEquipped = false;
        std::function<bool(Player &, Item &)> onUse;
        std::function<bool(Player &, Item &, const BlockPos &, uint32_t)> onUseOnBlock;
    };
}
