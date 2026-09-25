#pragma once

#include "Core/Math/Vector3i.h"
#include "Level/Generator/Feature/BlockManager.h"

#include <cstdint>
#include <vector>

class ServerNetworkHandler;

namespace garden {
    class GrowthBlockManager : public BlockManager {
    public:
        explicit GrowthBlockManager(Level &level);

        void setBlockStateAt(int32_t x, int32_t y, int32_t z, const BlockState &state) override;

        bool apply(ServerNetworkHandler &owner);

    private:
        std::vector<Vector3i> mChanged;
    };
}
