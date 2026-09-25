#pragma once

#include "falcon/Events.hpp"
#include "falcon/Item.hpp"
#include "falcon/Player.hpp"
#include "falcon/Vec3.hpp"

#include <string>

namespace falcon {
    class PlayerDeathEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_DEATH;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        std::string message() const {
            return detail::text(detail::api().eventMessage(mHandle));
        }

        void setMessage(const std::string &message) const {
            detail::api().eventSetMessage(mHandle, message.c_str());
        }
    };

    class PlayerRespawnEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_RESPAWN;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Vec3 position() const {
            return Vec3::from(detail::api().eventTo(mHandle));
        }

        void setPosition(const Vec3 &position) const {
            detail::api().eventSetTo(mHandle, position.raw());
        }
    };

    class PlayerMoveEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_MOVE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
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
    };

    class PlayerDropItemEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_DROP_ITEM;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Item item() const {
            return Item::borrow(detail::api().eventItem(mHandle));
        }
    };

    class PlayerPickupItemEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_PICKUP_ITEM;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Item item() const {
            return Item::borrow(detail::api().eventItem(mHandle));
        }
    };
}
