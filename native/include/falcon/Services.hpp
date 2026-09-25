#pragma once

#include "falcon/Api.hpp"
#include "falcon/Events.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace falcon {
    class CustomEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_CUSTOM;

        using Event::Event;

        std::string name() const {
            return detail::text(detail::api().eventName(mHandle));
        }

        std::string data() const {
            return detail::text(detail::api().eventData(mHandle));
        }

        void setData(const std::string &data) const {
            detail::api().eventSetData(mHandle, data.c_str());
        }

        std::string source() const {
            return detail::text(detail::api().eventCause(mHandle));
        }
    };

    struct CustomEventResult {
        bool cancelled = false;
        std::string data;
    };

    class Services {
    public:
        using Handler = std::function<std::string(const std::string &request)>;
        using CustomHandler = std::function<void(CustomEvent &event)>;

        bool provide(const std::string &name, Handler handler) const {
            auto *stored = detail::keep(std::make_unique<Handler>(std::move(handler)));
            return detail::api().registerService(detail::plugin(), name.c_str(), &serve, stored) != 0;
        }

        void withdraw(const std::string &name) const {
            detail::api().unregisterService(detail::plugin(), name.c_str());
        }

        bool has(const std::string &name) const {
            return detail::api().hasService(name.c_str()) != 0;
        }

        std::optional<std::string> provider(const std::string &name) const {
            const char *plugin = detail::api().serviceProvider(name.c_str());
            if (plugin == nullptr)
                return std::nullopt;
            return std::string(plugin);
        }

        std::optional<std::string> call(const std::string &name, const std::string &request = std::string()) const {
            int found = 0;
            const char *response = detail::api().callService(name.c_str(), request.c_str(), &found);
            if (found == 0)
                return std::nullopt;
            return detail::text(response);
        }

        uint64_t on(const std::string &name, CustomHandler handler, EventPriority priority = EventPriority::Normal,
                    bool ignoreCancelled = false) const {
            auto *stored = detail::keep(std::make_unique<CustomHandler>(std::move(handler)));
            return detail::api().subscribeCustomEvent(detail::plugin(), name.c_str(), (FalconEventPriority) priority,
                                                      ignoreCancelled ? 1 : 0, &dispatch, stored);
        }

        CustomEventResult fire(const std::string &name, const std::string &data = std::string(),
                               bool cancellable = false) const {
            int cancelled = 0;
            const char *result = detail::api().fireCustomEvent(detail::plugin(), name.c_str(), data.c_str(),
                                                               cancellable ? 1 : 0, &cancelled);
            CustomEventResult outcome;
            outcome.cancelled = cancelled != 0;
            outcome.data = detail::text(result);
            return outcome;
        }

    private:
        static const char *serve(const char *request, void *userData) {
            thread_local std::string response;
            response.clear();
            detail::guarded("A service handler threw an exception", [&] {
                response = (*static_cast<Handler *>(userData))(detail::text(request));
            });
            return response.c_str();
        }

        static void dispatch(FalconEvent *handle, void *userData) {
            detail::guarded("A custom event handler threw an exception", [&] {
                CustomEvent event(handle);
                (*static_cast<CustomHandler *>(userData))(event);
            });
        }
    };
}
