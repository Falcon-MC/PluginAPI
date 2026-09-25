#pragma once

#include "falcon/Events.hpp"

#include <cstdint>
#include <string>

namespace falcon {
    class ServerTickEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_SERVER_TICK;

        using Event::Event;

        uint64_t tick() const {
            return detail::api().eventTick(mHandle);
        }
    };

    class ServerCommandEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_SERVER_COMMAND;

        using Event::Event;

        std::string command() const {
            return detail::text(detail::api().eventMessage(mHandle));
        }

        void setCommand(const std::string &command) const {
            detail::api().eventSetMessage(mHandle, command.c_str());
        }
    };
}
