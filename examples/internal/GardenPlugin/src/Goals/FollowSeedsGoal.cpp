#include "GardenPlugin/Goals/FollowSeedsGoal.h"

#include "Actor/Mob/MobActor.h"
#include "Actor/ServerPlayer.h"
#include "Network/Handler/ServerNetworkHandler.h"

namespace garden {
    namespace {
        const char *SEEDS = "minecraft:wheat_seeds";
        const float STOP_DISTANCE_SQUARED = 4.0f;
        const int32_t REPATH_INTERVAL = 10;

        bool isHoldingSeeds(const ServerPlayer &player) {
            const ItemStack &held = player.getInventory().getItemInHand();
            return !held.isAir() && held.mDefinition->getIdentifier() == SEEDS;
        }
    }

    FollowSeedsGoal::FollowSeedsGoal(float speed, float range) : mSpeed(speed), mRange(range) {
        setRequiredControlFlags((uint8_t) GoalControlFlag::Move | (uint8_t) GoalControlFlag::Look);
    }

    bool FollowSeedsGoal::canUse(ServerNetworkHandler &owner, MobActor &mob) {
        return _findTempter(owner, mob) != nullptr;
    }

    void FollowSeedsGoal::start(ServerNetworkHandler &owner, MobActor &mob) {
        (void) owner;
        mTicksUntilRepath = 0;
        mob.getLookControl().setPitchEnabled(true);
    }

    void FollowSeedsGoal::stop(ServerNetworkHandler &owner, MobActor &mob) {
        (void) owner;
        mob.getNavigation().stop(mob);
        mob.getLookControl().clear();
        mob.getLookControl().setPitchEnabled(false);
    }

    void FollowSeedsGoal::tick(ServerNetworkHandler &owner, MobActor &mob) {
        const ServerPlayer *tempter = _findTempter(owner, mob);
        if (tempter == nullptr)
            return;

        mob.getLookControl().setLookAt(tempter->getPosition());

        if (mob.distanceSquaredTo(*tempter) <= STOP_DISTANCE_SQUARED) {
            mob.getNavigation().stop(mob);
            return;
        }

        if (--mTicksUntilRepath > 0)
            return;

        mTicksUntilRepath = REPATH_INTERVAL;
        mob.getNavigation().moveTo(tempter->getPosition(), mSpeed);
    }

    ServerPlayer *FollowSeedsGoal::_findTempter(ServerNetworkHandler &owner, const MobActor &mob) const {
        ServerPlayer *nearest = nullptr;
        float nearestDistance = mRange * mRange;

        for (auto &entry: owner.getPlayers()) {
            ServerPlayer &player = entry.second;
            if (!player.isSpawned() || player.isDead() || player.getDimension() != mob.getDimension())
                continue;

            const float distance = mob.distanceSquaredTo(player);
            if (distance > nearestDistance || !isHoldingSeeds(player))
                continue;

            nearest = &player;
            nearestDistance = distance;
        }

        return nearest;
    }
}
