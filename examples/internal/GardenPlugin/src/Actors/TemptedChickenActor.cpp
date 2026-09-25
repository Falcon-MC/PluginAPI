#include "GardenPlugin/Actors/TemptedChickenActor.h"

#include "Actor/AI/Goal/FloatGoal.h"
#include "Actor/AI/Goal/LookAtPlayerGoal.h"
#include "Actor/AI/Goal/PanicGoal.h"
#include "Actor/AI/Goal/RandomStrollGoal.h"
#include "Actor/ActorClassRegistry.h"
#include "GardenPlugin/Goals/FollowSeedsGoal.h"

#include <memory>

namespace garden {
    namespace {
        const int32_t FLOAT_PRIORITY = 0;

        const int32_t PANIC_PRIORITY = 1;
        const float PANIC_SPEED = 0.4f;
        const int32_t PANIC_RANGE = 12;
        const int32_t PANIC_INTERVAL = 40;
        const int32_t PANIC_DURATION = 100;

        const int32_t FOLLOW_PRIORITY = 3;
        const float FOLLOW_SPEED = 0.25f;
        const float FOLLOW_RANGE = 10.0f;

        const int32_t STROLL_PRIORITY = 6;
        const float STROLL_SPEED = 0.2f;
        const int32_t STROLL_RANGE = 12;
        const int32_t STROLL_INTERVAL = 100;
        const int32_t STROLL_WATER_RETRIES = 10;

        const int32_t LOOK_PRIORITY = 7;
        const float LOOK_RANGE = 6.0f;
        const int32_t LOOK_PROBABILITY = 4;
        const int32_t LOOK_PROBABILITY_TOTAL = 10;
        const int32_t LOOK_DURATION = 100;
        const int32_t LOOK_CHECK_INTERVAL = 100;
    }

    FALCON_REGISTER_ACTOR(TemptedChickenActor, ChickenActor::IDENTIFIER);

    void TemptedChickenActor::registerGoals(GoalSelector &goalSelector) {
        goalSelector.addGoal(FLOAT_PRIORITY, std::make_unique<FloatGoal>());
        goalSelector.addGoal(PANIC_PRIORITY, std::make_unique<PanicGoal>(PANIC_SPEED, PANIC_RANGE, PANIC_INTERVAL,
                                                                         PANIC_DURATION, true, STROLL_WATER_RETRIES));
        goalSelector.addGoal(FOLLOW_PRIORITY, std::make_unique<FollowSeedsGoal>(FOLLOW_SPEED, FOLLOW_RANGE));
        goalSelector.addGoal(STROLL_PRIORITY, std::make_unique<RandomStrollGoal>(STROLL_SPEED, STROLL_RANGE,
                                                                                 STROLL_INTERVAL, true,
                                                                                 STROLL_WATER_RETRIES));
        goalSelector.addGoal(LOOK_PRIORITY, std::make_unique<LookAtPlayerGoal>(LOOK_RANGE, LOOK_PROBABILITY,
                                                                               LOOK_PROBABILITY_TOTAL, LOOK_DURATION,
                                                                               LOOK_CHECK_INTERVAL));
    }
}
