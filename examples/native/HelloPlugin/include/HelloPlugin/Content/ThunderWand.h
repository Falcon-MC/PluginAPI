#pragma once

#include <falcon/Falcon.hpp>

class ThunderWand {
public:
    static constexpr const char *IDENTIFIER = "hello:thunder_wand";

    static void registerTo(const falcon::Plugin &plugin);
};
