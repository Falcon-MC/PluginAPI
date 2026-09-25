#pragma once

#include "HelloPlugin/Storage/JoinStore.h"

#include <falcon/Falcon.hpp>

class HelloPlugin : public falcon::Plugin {
public:
    void onLoad() override;

    bool onEnable() override;

    void onDisable() override;

private:
    JoinStore mJoins;
};
