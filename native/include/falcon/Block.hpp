#pragma once

#include "falcon/BlockPos.hpp"

#include <string>

namespace falcon {
    struct Block {
        BlockPos position;
        std::string name;
        std::string statesJson;
    };
}
