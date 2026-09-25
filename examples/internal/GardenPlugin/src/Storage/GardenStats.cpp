#include "GardenPlugin/Storage/GardenStats.h"

namespace garden {
    namespace {
        uint32_t gTreesGrown = 0;
    }

    void GardenStats::recordTree() {
        gTreesGrown++;
    }

    uint32_t GardenStats::getTreesGrown() {
        return gTreesGrown;
    }
}
