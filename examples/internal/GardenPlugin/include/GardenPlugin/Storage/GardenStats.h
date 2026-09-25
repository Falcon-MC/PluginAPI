#pragma once

#include <cstdint>

namespace garden {
    class GardenStats {
    public:
        static void recordTree();

        static uint32_t getTreesGrown();
    };
}
