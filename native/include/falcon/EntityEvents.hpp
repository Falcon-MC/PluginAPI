#pragma once

#include "falcon/Entity.hpp"
#include "falcon/Events.hpp"
#include "falcon/Player.hpp"

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
}
