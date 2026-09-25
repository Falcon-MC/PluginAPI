#pragma once

#include "falcon/Events.hpp"
#include "falcon/Player.hpp"

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace falcon {
    class DataPacketEvent : public Event {
    public:
        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        uint32_t packetId() const {
            return detail::api().eventPacketId(mHandle);
        }

        std::string_view dataView() const {
            uint32_t length = 0;
            const uint8_t *bytes = detail::api().eventPacketData(mHandle, &length);
            if (bytes == nullptr)
                return std::string_view();
            return std::string_view(reinterpret_cast<const char *>(bytes), length);
        }

        std::vector<uint8_t> data() const {
            uint32_t length = 0;
            const uint8_t *bytes = detail::api().eventPacketData(mHandle, &length);
            if (bytes == nullptr)
                return std::vector<uint8_t>();
            return std::vector<uint8_t>(bytes, bytes + length);
        }

        void setData(const uint8_t *data, std::size_t length) const {
            detail::api().eventSetPacketData(mHandle, data, static_cast<uint32_t>(length));
        }

        void setData(const std::vector<uint8_t> &data) const {
            setData(data.data(), data.size());
        }

        void setData(std::string_view data) const {
            setData(reinterpret_cast<const uint8_t *>(data.data()), data.size());
        }
    };

    class DataPacketReceiveEvent : public DataPacketEvent {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_DATA_PACKET_RECEIVE;

        using DataPacketEvent::DataPacketEvent;
    };

    class DataPacketSendEvent : public DataPacketEvent {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_DATA_PACKET_SEND;

        using DataPacketEvent::DataPacketEvent;
    };
}
