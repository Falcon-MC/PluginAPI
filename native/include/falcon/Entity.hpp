#pragma once

#include "falcon/Api.hpp"
#include "falcon/Effects.hpp"
#include "falcon/Vec3.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

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

        bool isInvisible() const {
            return detail::api().entityIsInvisible(mHandle) != 0;
        }

        /**
         * Hides the entity from every viewer, its name tag included, until it is made visible again.
         */
        void setInvisible(bool invisible) const {
            detail::api().entitySetInvisible(mHandle, invisible ? 1 : 0);
        }

        void extinguish() const {
            setOnFire(0);
        }

        /**
         * Adds a potion effect for duration ticks, or Effect::INFINITE. A weaker or shorter effect than the one
         * running is refused, as in the game.
         */
        bool addEffect(EffectType type, int32_t duration, int32_t amplifier = 0, bool showParticles = true) const {
            return detail::api().entityAddEffect(mHandle, static_cast<FalconEffectType>(type), duration, amplifier,
                                                 showParticles ? 1 : 0) != 0;
        }

        bool removeEffect(EffectType type) const {
            return detail::api().entityRemoveEffect(mHandle, static_cast<FalconEffectType>(type)) != 0;
        }

        std::optional<Effect> effect(EffectType type) const {
            FalconEffect value{};
            if (detail::api().entityEffect(mHandle, static_cast<FalconEffectType>(type), &value) == 0)
                return std::nullopt;
            return Effect::from(value);
        }

        bool hasEffect(EffectType type) const {
            return effect(type).has_value();
        }

        std::vector<Effect> effects() const {
            std::vector<Effect> result;
            const uint32_t count = detail::api().entityEffectCount(mHandle);
            for (uint32_t index = 0; index < count; ++index) {
                FalconEffect value{};
                if (detail::api().entityEffectAt(mHandle, index, &value) != 0)
                    result.push_back(Effect::from(value));
            }
            return result;
        }

        void clearEffects() const {
            detail::api().entityClearEffects(mHandle);
        }

        /**
         * An attribute by its identifier, such as minecraft:movement or minecraft:health. Only the attributes the
         * entity has are found: attributes() lists them.
         */
        std::optional<Attribute> attribute(const std::string &name) const {
            FalconAttribute value{};
            if (detail::api().entityAttribute(mHandle, name.c_str(), &value) == 0)
                return std::nullopt;
            return Attribute{name, value.value, value.minimum, value.maximum, value.defaultValue};
        }

        std::vector<Attribute> attributes() const {
            std::vector<Attribute> result;
            const uint32_t count = detail::api().entityAttributeCount(mHandle);
            for (uint32_t index = 0; index < count; ++index) {
                const std::string name = detail::text(detail::api().entityAttributeName(mHandle, index));
                std::optional<Attribute> found = attribute(name);
                if (found.has_value())
                    result.push_back(*found);
            }
            return result;
        }

        /** False when the entity has no such attribute or the value is outside its range. */
        bool setAttribute(const std::string &name, float value) const {
            return detail::api().entitySetAttribute(mHandle, name.c_str(), value) != 0;
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
