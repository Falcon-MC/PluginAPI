#pragma once

#include "falcon/Commands.hpp"
#include "falcon/Events.hpp"
#include "falcon/Logger.hpp"
#include "falcon/Scheduler.hpp"

#include <memory>
#include <string>

namespace falcon {
    class Plugin {
    public:
        virtual ~Plugin() = default;

        virtual void onLoad() {
        }

        virtual bool onEnable() {
            return true;
        }

        virtual void onDisable() {
        }

        std::string name() const {
            return detail::api().pluginName(detail::plugin());
        }

        std::string dataFolder() const {
            return detail::api().pluginDataFolder(detail::plugin());
        }

        const Logger &logger() const {
            return mLogger;
        }

        const Events &events() const {
            return mEvents;
        }

        const Commands &commands() const {
            return mCommands;
        }

        const Scheduler &scheduler() const {
            return mScheduler;
        }

    private:
        Logger mLogger;
        Events mEvents;
        Commands mCommands;
        Scheduler mScheduler;
    };

    namespace detail {
        inline std::unique_ptr<Plugin> gInstance;

        inline void onLoad(void *) {
            guarded("onLoad threw an exception", [] {
                gInstance->onLoad();
            });
        }

        inline int onEnable(void *) {
            int result = 0;
            guarded("onEnable threw an exception", [&] {
                result = gInstance->onEnable() ? 1 : 0;
            });
            return result;
        }

        inline void onDisable(void *) {
            guarded("onDisable threw an exception", [] {
                gInstance->onDisable();
            });
        }

        template<typename T>
        int entry(const FalconServerApi *api, FalconPlugin *plugin, FalconPluginCallbacks *callbacks) {
            if (api == nullptr || api->versionMajor != FALCON_API_VERSION_MAJOR)
                return 0;

            if (api->size < sizeof(FalconServerApi))
                return 0;

            gApi = api;
            gPlugin = plugin;
            guarded("The plugin constructor threw an exception", [] {
                gInstance = std::make_unique<T>();
            });
            if (gInstance == nullptr)
                return 0;

            callbacks->onLoad = &onLoad;
            callbacks->onEnable = &onEnable;
            callbacks->onDisable = &onDisable;
            callbacks->userData = nullptr;
            return 1;
        }
    }
}

#define FALCON_PLUGIN(Type) \
    extern "C" FALCON_EXPORT int falcon_plugin_entry(const FalconServerApi *api, FalconPlugin *plugin, \
                                                     FalconPluginCallbacks *callbacks) { \
        return falcon::detail::entry<Type>(api, plugin, callbacks); \
    }
