#pragma once

#include <falcon/Falcon.hpp>

class GiveDiamondCommand {
public:
    static constexpr const char *DIAMOND = "minecraft:diamond";

    static constexpr const char *PERMISSION = "hello.command.give-diamond";

    static void registerTo(const falcon::Plugin &plugin);
};
