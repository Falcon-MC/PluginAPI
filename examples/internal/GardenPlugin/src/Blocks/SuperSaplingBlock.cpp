#include "GardenPlugin/Blocks/SuperSaplingBlock.h"

#include "Actor/ServerPlayer.h"
#include "Block/BlockClassRegistry.h"
#include "GardenPlugin/Blocks/GrowthBlockManager.h"
#include "GardenPlugin/Storage/GardenStats.h"
#include "Level/Generator/Feature/Tree/LegacyTreeObject.h"
#include "Level/Generator/Random/SimpleRandom.h"
#include "Network/Handler/ServerNetworkHandler.h"

#include <chrono>

namespace garden {
    FALCON_REGISTER_BLOCK(SuperSaplingBlock, 10);

    bool SuperSaplingBlock::matches(const std::string &identifier) {
        return SaplingBlock::matches(identifier);
    }

    bool SuperSaplingBlock::onInteract(ServerNetworkHandler &owner, ServerPlayer &player, const Vector3i &position,
                                       const BlockState &state) const {
        (void) state;

        if (!player.getInventory().getItemInHand().isAir())
            return false;

        return grow(owner, owner.getLevelFor(player), position);
    }

    bool SuperSaplingBlock::grow(ServerNetworkHandler &owner, Level &level, const Vector3i &position) const {
        GrowthBlockManager manager(level);
        SimpleRandom random((int64_t) std::chrono::steady_clock::now().time_since_epoch().count());
        LegacyTreeObject::growTree(manager, position.x, position.y, position.z, random, getTreeType(), false);

        if (!manager.apply(owner))
            return false;

        GardenStats::recordTree();
        return true;
    }

    TreeWoodType SuperSaplingBlock::getTreeType() const {
        const std::string &identifier = getIdentifier();

        if (identifier == "minecraft:spruce_sapling")
            return TreeWoodType::SPRUCE;
        if (identifier == "minecraft:birch_sapling")
            return TreeWoodType::BIRCH;
        if (identifier == "minecraft:jungle_sapling")
            return TreeWoodType::JUNGLE;
        if (identifier == "minecraft:acacia_sapling")
            return TreeWoodType::ACACIA;
        if (identifier == "minecraft:dark_oak_sapling")
            return TreeWoodType::DARK_OAK;
        if (identifier == "minecraft:cherry_sapling")
            return TreeWoodType::CHERRY;
        if (identifier == "minecraft:pale_oak_sapling")
            return TreeWoodType::PALE_OAK;

        return TreeWoodType::OAK;
    }
}
