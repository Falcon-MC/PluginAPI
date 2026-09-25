#pragma once

#include <falcon/internal/InternalPlugin.hpp>

namespace garden {
    class JoinListener {
    public:
        static void registerTo(falcon::internal::Plugin &plugin);
    };
}
