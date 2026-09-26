#pragma once

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
}
