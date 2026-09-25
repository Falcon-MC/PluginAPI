#pragma once

#include "falcon/Player.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace falcon {
    enum class EventPriority : uint32_t {
        Lowest = FALCON_PRIORITY_LOWEST,
        Low = FALCON_PRIORITY_LOW,
        Normal = FALCON_PRIORITY_NORMAL,
        High = FALCON_PRIORITY_HIGH,
        Highest = FALCON_PRIORITY_HIGHEST,
        Monitor = FALCON_PRIORITY_MONITOR
    };

    class Event {
    public:
        explicit Event(FalconEvent *handle) : mHandle(handle) {
        }

        bool isCancelled() const {
            return detail::api().eventIsCancelled(mHandle) != 0;
        }

        void setCancelled(bool cancelled) const {
            detail::api().eventSetCancelled(mHandle, cancelled ? 1 : 0);
        }

        void cancel() const {
            setCancelled(true);
        }

    protected:
        FalconEvent *mHandle;
    };

    class PlayerJoinEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_JOIN;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }
    };

    class PlayerQuitEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_QUIT;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }
    };

    class PlayerChatEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PLAYER_CHAT;

        using Event::Event;

        Player player() const {
            return Player(detail::api().eventPlayer(mHandle));
        }

        std::string message() const {
            return detail::api().eventMessage(mHandle);
        }

        void setMessage(const std::string &message) const {
            detail::api().eventSetMessage(mHandle, message.c_str());
        }
    };

    class Events {
    public:
        template<typename T>
        uint64_t on(std::function<void(T &)> handler, EventPriority priority = EventPriority::Normal,
                    bool ignoreCancelled = false) const {
            auto *stored = detail::keep(std::make_unique<std::function<void(T &)>>(std::move(handler)));
            return detail::api().subscribe(detail::plugin(), T::TYPE, (FalconEventPriority) priority,
                                           ignoreCancelled ? 1 : 0, &dispatch<T>, stored);
        }

        void off(uint64_t subscription) const {
            detail::api().unsubscribe(subscription);
        }

    private:
        template<typename T>
        static void dispatch(FalconEvent *handle, void *userData) {
            T event(handle);
            (*static_cast<std::function<void(T &)> *>(userData))(event);
        }
    };
}
