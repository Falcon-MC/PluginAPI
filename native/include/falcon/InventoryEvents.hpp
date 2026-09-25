#pragma once

#include "falcon/BlockPos.hpp"
#include "falcon/Events.hpp"
#include "falcon/Item.hpp"
#include "falcon/Level.hpp"
#include "falcon/Player.hpp"

#include <cstdint>
#include <string>

namespace falcon {
    class InventoryOpenEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_INVENTORY_OPEN;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        std::string blockName() const {
            return detail::text(detail::api().eventBlockName(mHandle));
        }
    };

    class InventoryCloseEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_INVENTORY_CLOSE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        std::string blockName() const {
            return detail::text(detail::api().eventBlockName(mHandle));
        }
    };

    class InventoryTransactionEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_INVENTORY_TRANSACTION;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        std::string action() const {
            return detail::text(detail::api().eventCause(mHandle));
        }

        Item item() const {
            return Item::borrow(detail::api().eventItem(mHandle));
        }

        uint32_t count() const {
            return (uint32_t) detail::api().eventAmount(mHandle);
        }

        std::string sourceContainer() const {
            return detail::text(detail::api().eventSourceContainer(mHandle));
        }

        int32_t sourceSlot() const {
            return detail::api().eventSourceSlot(mHandle);
        }

        std::string destinationContainer() const {
            return detail::text(detail::api().eventDestinationContainer(mHandle));
        }

        int32_t destinationSlot() const {
            return detail::api().eventDestinationSlot(mHandle);
        }
    };

    class CraftItemEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_CRAFT_ITEM;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        Item result() const {
            return Item::borrow(detail::api().eventItem(mHandle));
        }

        uint32_t times() const {
            return (uint32_t) detail::api().eventAmount(mHandle);
        }
    };
}
