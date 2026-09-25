#pragma once

#include "Actor/Mob/Passive/ChickenActor.h"

namespace garden {
    class TemptedChickenActor : public ChickenActor {
    public:
        using ChickenActor::ChickenActor;

    protected:
        void registerGoals(GoalSelector &goalSelector) override;
    };
}
