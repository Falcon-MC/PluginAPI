#pragma once

#include "falcon/BlockPos.hpp"
#include "falcon/Events.hpp"
#include "falcon/Item.hpp"
#include "falcon/Level.hpp"
#include "falcon/Player.hpp"
#include "falcon/Vec3.hpp"

#include <cstdint>
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

    class PlayerCommandEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_COMMAND;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        std::string command() const {
            return detail::text(detail::api().eventMessage(mHandle));
        }

        void setCommand(const std::string &command) const {
            detail::api().eventSetMessage(mHandle, command.c_str());
        }
    };

    class PlayerGameModeChangeEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_GAME_MODE_CHANGE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        GameMode gameMode() const {
            return static_cast<GameMode>(detail::api().eventGameMode(mHandle));
        }

        GameMode previousGameMode() const {
            return static_cast<GameMode>(detail::api().eventPreviousGameMode(mHandle));
        }
    };

    class PlayerChangeDimensionEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_CHANGE_DIMENSION;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Dimension dimension() const {
            return static_cast<Dimension>(detail::api().eventDimension(mHandle));
        }

        Dimension previousDimension() const {
            return static_cast<Dimension>(detail::api().eventPreviousDimension(mHandle));
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

    class PlayerPreLoginEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_PRE_LOGIN;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        std::string kickMessage() const {
            return detail::text(detail::api().eventMessage(mHandle));
        }

        void setKickMessage(const std::string &message) const {
            detail::api().eventSetMessage(mHandle, message.c_str());
        }
    };

    class PlayerToggleSneakEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_TOGGLE_SNEAK;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        bool isSneaking() const {
            return detail::api().eventState(mHandle) != 0;
        }
    };

    class PlayerToggleSprintEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_TOGGLE_SPRINT;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        bool isSprinting() const {
            return detail::api().eventState(mHandle) != 0;
        }
    };

    class PlayerToggleFlightEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_TOGGLE_FLIGHT;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        bool isFlying() const {
            return detail::api().eventState(mHandle) != 0;
        }
    };

    class PlayerItemHeldEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_ITEM_HELD;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        int32_t previousSlot() const {
            return detail::api().eventSourceSlot(mHandle);
        }

        int32_t newSlot() const {
            return detail::api().eventDestinationSlot(mHandle);
        }

        Item item() const {
            return Item::borrow(detail::api().eventItem(mHandle));
        }
    };

    class PlayerItemConsumeEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_ITEM_CONSUME;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Item item() const {
            return Item::borrow(detail::api().eventItem(mHandle));
        }
    };

    class PlayerBedEnterEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_BED_ENTER;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        BlockPos bed() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }
    };

    class PlayerBedLeaveEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_BED_LEAVE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        BlockPos bed() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }
    };

    class PlayerJumpEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_JUMP;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }
    };

    class PlayerFoodChangeEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_FOOD_CHANGE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        double food() const {
            return detail::api().eventAmount(mHandle);
        }

        void setFood(double food) const {
            detail::api().eventSetAmount(mHandle, food);
        }

        double previousFood() const {
            return detail::api().eventPreviousAmount(mHandle);
        }
    };

    class PlayerExperienceChangeEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_EXPERIENCE_CHANGE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        int32_t amount() const {
            return (int32_t) detail::api().eventAmount(mHandle);
        }

        void setAmount(int32_t amount) const {
            detail::api().eventSetAmount(mHandle, (double) amount);
        }
    };
}
