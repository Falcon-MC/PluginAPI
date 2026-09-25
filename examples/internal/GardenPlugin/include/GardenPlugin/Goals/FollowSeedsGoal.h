#pragma once

#include "Actor/AI/Goal/Goal.h"

#include <cstdint>

class ServerPlayer;

namespace garden {
    class FollowSeedsGoal : public Goal {
    public:
        FollowSeedsGoal(float speed, float range);

        bool canUse(ServerNetworkHandler &owner, MobActor &mob) override;

        void start(ServerNetworkHandler &owner, MobActor &mob) override;

        void stop(ServerNetworkHandler &owner, MobActor &mob) override;

        void tick(ServerNetworkHandler &owner, MobActor &mob) override;

    private:
        ServerPlayer *_findTempter(ServerNetworkHandler &owner, const MobActor &mob) const;

        float mSpeed;
        float mRange;
        int32_t mTicksUntilRepath = 0;
    };
}
