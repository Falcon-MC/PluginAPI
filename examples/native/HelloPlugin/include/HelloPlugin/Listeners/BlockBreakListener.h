#pragma once

#include <falcon/Falcon.hpp>

class BlockBreakListener {
public:
    static void registerTo(const falcon::Plugin &plugin);
};
