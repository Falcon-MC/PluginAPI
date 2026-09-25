#pragma once

#include <falcon/Falcon.hpp>

class JoinStore;

class JoinsCommand {
public:
    static void registerTo(const falcon::Plugin &plugin, JoinStore &joins);
};
