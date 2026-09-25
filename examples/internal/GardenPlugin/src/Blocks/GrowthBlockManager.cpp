#include "GardenPlugin/Blocks/GrowthBlockManager.h"

#include "Level/Level.h"
#include "Network/Handler/BlockActionHandler.h"

namespace garden {
    GrowthBlockManager::GrowthBlockManager(Level &level) : BlockManager(level) {
    }

    void GrowthBlockManager::setBlockStateAt(int32_t x, int32_t y, int32_t z, const BlockState &state) {
        BlockManager::setBlockStateAt(x, y, z, state);
        mChanged.push_back(Vector3i(x, y, z));
    }

    bool GrowthBlockManager::apply(ServerNetworkHandler &owner) {
        if (mChanged.empty())
            return false;

        const std::vector<Vector3i> changed = mChanged;
        applySubChunkUpdate();

        Level &level = getLevel();
        for (const Vector3i &position: changed) {
            const BlockState state = level.getBlockState(position.x, position.y, position.z);
            BlockActionHandler::broadcastBlockUpdate(owner, level, position, state);
        }

        return true;
    }
}
