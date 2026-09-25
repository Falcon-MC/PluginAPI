#include "GardenPlugin/Items/GardenWandItem.h"

#include "Actor/ServerPlayer.h"
#include "Block/Blocks/VanillaBlocks.h"
#include "GardenPlugin/Blocks/SuperSaplingBlock.h"
#include "Item/ItemClassRegistry.h"
#include "Level/Level.h"
#include "Network/Handler/ServerNetworkHandler.h"

#include <string>

namespace garden {
    namespace {
        const int32_t HORIZONTAL_RADIUS = 2;
        const int32_t VERTICAL_RADIUS = 1;
    }

    FALCON_REGISTER_ITEM(GardenWandItem, 5);

    GardenWandItem::GardenWandItem(const Item &base) : Item(base) {
    }

    bool GardenWandItem::matches(const std::string &identifier) {
        return identifier == "minecraft:stick";
    }

    bool GardenWandItem::onUseOnBlock(ServerNetworkHandler &owner, ServerPlayer &player, const ItemStack &item,
                                      const Vector3i &blockPosition, int32_t face,
                                      const Vector3f &clickPosition) const {
        (void) item;
        (void) face;
        (void) clickPosition;

        Level &level = owner.getLevelFor(player);
        uint32_t grown = 0;

        for (int32_t x = -HORIZONTAL_RADIUS; x <= HORIZONTAL_RADIUS; x++) {
            for (int32_t y = -VERTICAL_RADIUS; y <= VERTICAL_RADIUS; y++) {
                for (int32_t z = -HORIZONTAL_RADIUS; z <= HORIZONTAL_RADIUS; z++) {
                    const Vector3i position(blockPosition.x + x, blockPosition.y + y, blockPosition.z + z);
                    const BlockState state = level.getBlockState(position.x, position.y, position.z);
                    const SuperSaplingBlock *sapling = VanillaBlocks::getAs<SuperSaplingBlock>(state.mName);
                    if (sapling != nullptr && sapling->grow(owner, level, position))
                        grown++;
                }
            }
        }

        if (grown == 0)
            return false;

        player.sendMessage("The garden wand grew " + std::to_string(grown) + " tree(s)");
        return true;
    }
}
