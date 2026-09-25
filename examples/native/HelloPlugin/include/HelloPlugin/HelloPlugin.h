#pragma once

#include "HelloPlugin/Storage/JoinStore.h"

#include <falcon/Falcon.hpp>

class HelloPlugin : public falcon::Plugin {
public:
    bool onEnable() override;

    void onDisable() override;

private:
    JoinStore mJoins;
};
