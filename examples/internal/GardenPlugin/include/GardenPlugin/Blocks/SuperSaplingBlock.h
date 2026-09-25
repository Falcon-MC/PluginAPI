#pragma once

#include "Block/Blocks/SaplingBlock.h"
#include "Level/Generator/Feature/Tree/TreeWoodType.h"

#include <string>

class Level;
class ServerNetworkHandler;
class ServerPlayer;

namespace garden {
    class SuperSaplingBlock : public SaplingBlock {
    public:
        using SaplingBlock::SaplingBlock;

        static bool matches(const std::string &identifier);

        bool onInteract(ServerNetworkHandler &owner, ServerPlayer &player, const Vector3i &position,
                        const BlockState &state) const override;

        bool grow(ServerNetworkHandler &owner, Level &level, const Vector3i &position) const;

    private:
        TreeWoodType getTreeType() const;
    };
}
