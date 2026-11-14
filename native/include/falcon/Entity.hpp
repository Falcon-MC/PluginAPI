#pragma once

#include "falcon/Api.hpp"
#include "falcon/Effects.hpp"
#include "falcon/Item.hpp"
#include "falcon/Vec3.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace falcon {
    class Level;
    class Player;

    enum class EquipmentSlot : uint32_t {
        Mainhand = FALCON_EQUIPMENT_MAINHAND,
        Offhand = FALCON_EQUIPMENT_OFFHAND,
        Head = FALCON_EQUIPMENT_HEAD,
        Chest = FALCON_EQUIPMENT_CHEST,
        Legs = FALCON_EQUIPMENT_LEGS,
        Feet = FALCON_EQUIPMENT_FEET,
        Body = FALCON_EQUIPMENT_BODY
    };

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

        /**
         * Text this plugin keeps on the entity or player, saved with it. Keys belong to the plugin: another plugin
         * never sees them. Entities without saved state, such as dropped items, keep nothing.
         */
        std::optional<std::string> data(const std::string &key) const {
            const char *value = detail::api().entityData(detail::plugin(), mHandle, key.c_str());
            if (value == nullptr)
                return std::nullopt;
            return std::string(value);
        }

        bool setData(const std::string &key, const std::string &value) const {
            return detail::api().entitySetData(detail::plugin(), mHandle, key.c_str(), value.c_str()) != 0;
        }

        bool removeData(const std::string &key) const {
            return detail::api().entitySetData(detail::plugin(), mHandle, key.c_str(), nullptr) != 0;
        }

        std::vector<std::string> dataKeys() const {
            std::vector<std::string> keys;
            const uint32_t count = detail::api().entityDataCount(detail::plugin(), mHandle);
            for (uint32_t index = 0; index < count; index++)
                keys.push_back(detail::text(detail::api().entityDataKey(detail::plugin(), mHandle, index)));
            return keys;
        }

        /** What the server would save for this entity, as little-endian NBT; read only. */
        std::vector<uint8_t> nbt() const {
            uint32_t length = 0;
            const uint8_t *data = detail::api().entityNbt(mHandle, &length);
            if (data == nullptr)
                return {};
            return std::vector<uint8_t>(data, data + length);
        }

        /** What a mob wears or holds; nothing for an empty slot or an entity without equipment, players included. */
        std::optional<Item> equipment(EquipmentSlot slot) const {
            const FalconEquipmentSlot raw = static_cast<FalconEquipmentSlot>(slot);
            std::optional<Item> found = Item::adopt(detail::api().entityEquipment(mHandle, raw));
            if (found.has_value() && found->isEmpty())
                return std::nullopt;
            return found;
        }

        bool setEquipment(EquipmentSlot slot, const Item &item) const {
            const FalconEquipmentSlot raw = static_cast<FalconEquipmentSlot>(slot);
            return detail::api().entitySetEquipment(mHandle, raw, item.handle()) != 0;
        }

        bool clearEquipment(EquipmentSlot slot) const {
            return detail::api().entitySetEquipment(mHandle, static_cast<FalconEquipmentSlot>(slot), nullptr) != 0;
        }

        std::optional<Entity> vehicle() const {
            FalconEntity *ridden = detail::api().entityVehicle(mHandle);
            if (ridden == nullptr)
                return std::nullopt;
            return Entity(ridden);
        }

        std::vector<Entity> passengers() const {
            std::vector<Entity> result;
            const uint32_t count = detail::api().entityPassengerCount(mHandle);
            for (uint32_t index = 0; index < count; ++index) {
                FalconEntity *rider = detail::api().entityPassenger(mHandle, index);
                if (rider != nullptr)
                    result.emplace_back(rider);
            }
            return result;
        }

        /** Seats this entity on a vehicle that has a free seat, as when a player mounts it. */
        bool mount(const Entity &vehicle) const {
            return detail::api().entityMount(mHandle, vehicle.mHandle) != 0;
        }

        void dismount() const {
            detail::api().entityDismount(mHandle);
        }

        void ejectPassengers() const {
            detail::api().entityEjectPassengers(mHandle);
        }

        /** The tags of /tag, saved with the entity. */
        std::vector<std::string> tags() const {
            std::vector<std::string> result;
            const uint32_t count = detail::api().entityTagCount(mHandle);
            for (uint32_t index = 0; index < count; ++index)
                result.push_back(detail::text(detail::api().entityTag(mHandle, index)));
            return result;
        }

        bool hasTag(const std::string &tag) const {
            return detail::api().entityHasTag(mHandle, tag.c_str()) != 0;
        }

        bool addTag(const std::string &tag) const {
            return detail::api().entityAddTag(mHandle, tag.c_str()) != 0;
        }

        bool removeTag(const std::string &tag) const {
            return detail::api().entityRemoveTag(mHandle, tag.c_str()) != 0;
        }

        /** The entity a mob is attacking. */
        std::optional<Entity> attackTarget() const {
            FalconEntity *target = detail::api().entityAttackTarget(mHandle);
            if (target == nullptr)
                return std::nullopt;
            return Entity(target);
        }

        /** Makes a mob attack an entity; EntityTargetEvent listeners can refuse it. */
        bool setAttackTarget(const Entity &target) const {
            return detail::api().entitySetAttackTarget(mHandle, target.mHandle) != 0;
        }

        bool clearAttackTarget() const {
            return detail::api().entitySetAttackTarget(mHandle, nullptr) != 0;
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
