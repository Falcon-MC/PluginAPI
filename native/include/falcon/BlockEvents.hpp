#pragma once

#include "falcon/BlockPos.hpp"
#include "falcon/Events.hpp"
#include "falcon/Player.hpp"

#include <cstdint>
#include <string>

namespace falcon {
    class BlockBreakEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_BLOCK_BREAK;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        std::string blockName() const {
            return detail::text(detail::api().eventBlockName(mHandle));
        }
    };

    class BlockPlaceEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_BLOCK_PLACE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        std::string blockName() const {
            return detail::text(detail::api().eventBlockName(mHandle));
        }
    };

    class PlayerInteractBlockEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_INTERACT_BLOCK;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        uint32_t face() const {
            return detail::api().eventBlockFace(mHandle);
        }
    };

    class SignChangeEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_SIGN_CHANGE;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        bool isFrontSide() const {
            return detail::api().eventState(mHandle) != 0;
        }

        std::string text() const {
            return detail::text(detail::api().eventMessage(mHandle));
        }

        void setText(const std::string &text) const {
            detail::api().eventSetMessage(mHandle, text.c_str());
        }
    };
}
