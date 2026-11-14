#pragma once

#include "falcon/Effects.hpp"
#include "falcon/Entity.hpp"
#include "falcon/Events.hpp"
#include "falcon/Level.hpp"
#include "falcon/Player.hpp"
#include "falcon/Vec3.hpp"

#include <optional>
#include <string>

namespace falcon {
    class PlayerInteractEntityEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_INTERACT_ENTITY;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }
    };

    class EntityDamageEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_DAMAGE;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> attacker() const {
            FalconEntity *source = detail::api().eventAttacker(mHandle);
            if (source == nullptr)
                return std::nullopt;
            return Entity(source);
        }

        double amount() const {
            return detail::api().eventAmount(mHandle);
        }

        void setAmount(double amount) const {
            detail::api().eventSetAmount(mHandle, amount);
        }

        std::string cause() const {
            return detail::text(detail::api().eventCause(mHandle));
        }
    };

    class EntityDeathEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_DEATH;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }
    };

    class EntitySpawnEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_SPAWN;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }

        Vec3 position() const {
            return Vec3::from(detail::api().eventPosition(mHandle));
        }
    };

    class EntityTransformEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_TRANSFORM;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        Entity result() const {
            return Entity(detail::api().eventTarget(mHandle));
        }
    };

    class EntityTargetEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_TARGET;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> target() const {
            FalconEntity *target = detail::api().eventTarget(mHandle);
            if (target == nullptr)
                return std::nullopt;
            return Entity(target);
        }
    };

    /** An entity or player gets a potion effect; cancellable. */
    class EntityEffectAddEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_EFFECT_ADD;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        Effect effect() const {
            FalconEffect value{};
            detail::api().eventEffect(mHandle, &value);
            return Effect::from(value);
        }
    };

    /** An entity or player regains health; cancellable, the amount can change. */
    class EntityHealEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_HEAL;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        double amount() const {
            return detail::api().eventAmount(mHandle);
        }

        void setAmount(double value) const {
            detail::api().eventSetAmount(mHandle, value);
        }

        std::string cause() const {
            return detail::text(detail::api().eventCause(mHandle));
        }
    };

    /** Two mobs in love make a baby; cancellable. */
    class EntityBreedEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_BREED;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> partner() const {
            FalconEntity *entity = detail::api().eventTarget(mHandle);
            if (entity == nullptr)
                return std::nullopt;
            return Entity(entity);
        }

        std::string babyType() const {
            return detail::text(detail::api().eventCause(mHandle));
        }
    };

    /** A mob is tamed; cancellable. */
    class EntityTameEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_TAME;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> tamer() const {
            FalconEntity *entity = detail::api().eventTarget(mHandle);
            if (entity == nullptr)
                return std::nullopt;
            return Entity(entity);
        }
    };

    /** An entity or player gets on a vehicle; cancellable. */
    class EntityMountEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_MOUNT;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> vehicle() const {
            FalconEntity *entity = detail::api().eventTarget(mHandle);
            if (entity == nullptr)
                return std::nullopt;
            return Entity(entity);
        }
    };

    /** An entity or player gets off a vehicle; not cancellable. */
    class EntityDismountEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_DISMOUNT;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> vehicle() const {
            FalconEntity *entity = detail::api().eventTarget(mHandle);
            if (entity == nullptr)
                return std::nullopt;
            return Entity(entity);
        }
    };

    /** An entity other than a player is moved by an end gateway, a pet following its owner or a plugin; cancellable. */
    class EntityTeleportEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_ENTITY_TELEPORT;

        using Event::Event;

        Entity entity() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        Vec3 from() const {
            return Vec3::from(detail::api().eventFrom(mHandle));
        }

        Vec3 to() const {
            return Vec3::from(detail::api().eventTo(mHandle));
        }

        void setTo(const Vec3 &position) const {
            detail::api().eventSetTo(mHandle, position.raw());
        }

        std::string cause() const {
            return detail::text(detail::api().eventCause(mHandle));
        }
    };
}
