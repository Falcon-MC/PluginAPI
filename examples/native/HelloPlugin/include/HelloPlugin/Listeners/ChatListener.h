#pragma once

#include <falcon/Falcon.hpp>

class ChatListener {
public:
    static void registerTo(const falcon::Plugin &plugin);
};
