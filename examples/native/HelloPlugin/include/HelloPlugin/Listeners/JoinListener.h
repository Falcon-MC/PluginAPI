#pragma once

#include <falcon/Falcon.hpp>

class JoinStore;

class JoinListener {
public:
    static void registerTo(const falcon::Plugin &plugin, JoinStore &joins);
};
