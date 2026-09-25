#pragma once

#include "falcon/Api.hpp"
#include "falcon/Vec3.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace falcon {
    class Level;
    class Player;

    class Entity {
    public:
        explicit Entity(FalconEntity *handle) : mHandle(handle) {
        }

        bool valid() const {
            return mHandle != nullptr;
        }

        std::string type() const {
            return detail::text(detail::api().entityType(mHandle));
        }

        uint64_t runtimeId() const {
            return detail::api().entityRuntimeId(mHandle);
        }

        Level level() const;

        Vec3 position() const {
            return Vec3::from(detail::api().entityPosition(mHandle));
        }

        Vec3 rotation() const {
            return Vec3::from(detail::api().entityRotation(mHandle));
        }

        void teleport(const Vec3 &position) const {
            detail::api().entityTeleport(mHandle, nullptr, position.raw());
        }

        void teleport(const Vec3 &position, const Level &level) const;

        Vec3 motion() const {
            return Vec3::from(detail::api().entityMotion(mHandle));
        }

        void setMotion(const Vec3 &motion) const {
            detail::api().entitySetMotion(mHandle, motion.raw());
        }

        float health() const {
            return detail::api().entityHealth(mHandle);
        }

        float maxHealth() const {
            return detail::api().entityMaxHealth(mHandle);
        }

        void setHealth(float health) const {
            detail::api().entitySetHealth(mHandle, health);
        }

        bool isAlive() const {
            return detail::api().entityIsAlive(mHandle) != 0;
        }

        bool damage(float amount, const std::string &cause = "") const {
            return detail::api().entityDamage(mHandle, amount, detail::nullable(cause), nullptr) != 0;
        }

        bool damage(float amount, const std::string &cause, const Entity &attacker) const {
            return detail::api().entityDamage(mHandle, amount, detail::nullable(cause), attacker.mHandle) != 0;
        }

        void kill() const {
            detail::api().entityKill(mHandle);
        }

        void remove() const {
            detail::api().entityRemove(mHandle);
        }

        std::string nameTag() const {
            return detail::text(detail::api().entityNameTag(mHandle));
        }

        void setNameTag(const std::string &nameTag) const {
            detail::api().entitySetNameTag(mHandle, nameTag.c_str());
        }

        bool isOnFire() const {
            return detail::api().entityIsOnFire(mHandle) != 0;
        }

        void setOnFire(uint32_t ticks) const {
            detail::api().entitySetOnFire(mHandle, ticks);
        }

        void extinguish() const {
            setOnFire(0);
        }

        std::optional<Player> asPlayer() const;

        FalconEntity *handle() const {
            return mHandle;
        }

    private:
        FalconEntity *mHandle;
    };
}

#include "falcon/Level.hpp"
#include "falcon/Player.hpp"

namespace falcon {
    inline Level Entity::level() const {
        return Level(detail::api().entityLevel(mHandle));
    }

    inline void Entity::teleport(const Vec3 &position, const Level &level) const {
        detail::api().entityTeleport(mHandle, level.handle(), position.raw());
    }

    inline std::optional<Player> Entity::asPlayer() const {
        FalconPlayer *player = detail::api().entityPlayer(mHandle);
        if (player == nullptr)
            return std::nullopt;
        return Player(player);
    }
}
