#pragma once

#include <falcon/internal/InternalPlugin.hpp>

namespace garden {
    class GardenPlugin : public falcon::internal::Plugin {
    public:
        void onLoad() override;

        bool onEnable() override;

        void onDisable() override;
    };
}
