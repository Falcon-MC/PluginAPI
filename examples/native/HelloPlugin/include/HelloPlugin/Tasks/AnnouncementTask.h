#pragma once

#include <falcon/Falcon.hpp>

#include <cstdint>

class AnnouncementTask {
public:
    static constexpr uint64_t PERIOD_TICKS = 20 * 60 * 5;

    static void start(falcon::Plugin &plugin);
};
