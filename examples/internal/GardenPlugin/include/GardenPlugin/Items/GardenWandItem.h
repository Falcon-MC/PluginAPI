#pragma once

#include "Item/Item.h"

#include <cstdint>
#include <string>

namespace garden {
    class GardenWandItem : public Item {
    public:
        explicit GardenWandItem(const Item &base);

        static bool matches(const std::string &identifier);

        bool onUseOnBlock(ServerNetworkHandler &owner, ServerPlayer &player, const ItemStack &item,
                          const Vector3i &blockPosition, int32_t face, const Vector3f &clickPosition) const override;
    };
}
