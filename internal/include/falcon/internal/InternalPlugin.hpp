#pragma once

#include "Command/Command.h"
#include "Command/CommandMap.h"
#include "Core/Event/EventBus.h"
#include "Network/Handler/ServerNetworkHandler.h"
#include "Plugin/InternalPluginBuild.h"
#include "Plugin/LoadedPlugin.h"
#include "Plugin/PluginRegistrationScope.h"

#include <falcon/falcon_api.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace falcon::internal {
    namespace detail {
        template<typename T>
        int entry(ServerNetworkHandler &server, LoadedPlugin &plugin, FalconPluginCallbacks *callbacks);
    }

    class Plugin {
    public:
        Plugin() = default;

        virtual ~Plugin() = default;

        Plugin(const Plugin &) = delete;

        Plugin &operator=(const Plugin &) = delete;

        virtual void onLoad() {
        }

        virtual bool onEnable() {
            return true;
        }

        virtual void onDisable() {
        }

        ServerNetworkHandler &getServer() const {
            return *mServer;
        }

        LoadedPlugin &getLoadedPlugin() const {
            return *mPlugin;
        }

        const std::string &getName() const {
            return mPlugin->mDescription.mName;
        }

        const std::string &getDataFolder() const {
            return mPlugin->mDataFolder;
        }

        bool registerCommand(std::shared_ptr<Command> command) {
            if (command == nullptr)
                return false;

            CommandMap &commands = mServer->getCommands();
            if (commands.getCommand(command->getName()) != nullptr)
                return false;

            const std::string name = command->getName();
            commands.registerCommand(std::move(command));
            mPlugin->mCommands.push_back(name);
            return true;
        }

        template<typename T>
        void subscribe(EventSignal<T> &signal, typename EventSignal<T>::Handler handler) {
            const uint32_t id = signal.subscribe(std::move(handler));
            mSubscriptions.push_back([&signal, id]() {
                signal.unsubscribe(id);
            });
        }

        void unsubscribeAll() {
            for (const std::function<void()> &unsubscribe: mSubscriptions)
                unsubscribe();

            mSubscriptions.clear();
        }

    private:
        template<typename T>
        friend int detail::entry(ServerNetworkHandler &server, LoadedPlugin &plugin,
                                 FalconPluginCallbacks *callbacks);

        ServerNetworkHandler *mServer = nullptr;
        LoadedPlugin *mPlugin = nullptr;
        std::vector<std::function<void()>> mSubscriptions;
    };

    namespace detail {
        inline void onLoad(void *userData) {
            Plugin &plugin = *static_cast<Plugin *>(userData);
            const PluginRegistrationScope scope(&plugin.getLoadedPlugin(), true);
            plugin.onLoad();
        }

        inline int onEnable(void *userData) {
            Plugin &plugin = *static_cast<Plugin *>(userData);
            const PluginRegistrationScope scope(&plugin.getLoadedPlugin(), true);

            bool enabled = false;
            try {
                enabled = plugin.onEnable();
            } catch (...) {
                plugin.unsubscribeAll();
                throw;
            }

            if (!enabled)
                plugin.unsubscribeAll();
            return enabled ? 1 : 0;
        }

        inline void onDisable(void *userData) {
            Plugin &plugin = *static_cast<Plugin *>(userData);

            try {
                plugin.onDisable();
            } catch (...) {
                plugin.unsubscribeAll();
                throw;
            }

            plugin.unsubscribeAll();
        }

        template<typename T>
        int entry(ServerNetworkHandler &server, LoadedPlugin &plugin, FalconPluginCallbacks *callbacks) {
            static_assert(std::is_base_of<Plugin, T>::value, "An internal plugin must derive from Plugin");

            std::shared_ptr<T> instance = std::make_shared<T>();
            Plugin &base = *instance;
            base.mServer = &server;
            base.mPlugin = &plugin;

            callbacks->onLoad = &onLoad;
            callbacks->onEnable = &onEnable;
            callbacks->onDisable = &onDisable;
            callbacks->userData = &base;
            plugin.mInstance = std::move(instance);
            return 1;
        }
    }
}

#define FALCON_INTERNAL_PLUGIN(Type) \
    extern "C" FALCON_EXPORT const InternalPluginBuild *falcon_internal_plugin_build() { \
        static const InternalPluginBuild build = FALCON_INTERNAL_BUILD; \
        return &build; \
    } \
    extern "C" FALCON_EXPORT int falcon_internal_plugin_entry(ServerNetworkHandler &server, LoadedPlugin &plugin, \
                                                              FalconPluginCallbacks *callbacks) { \
        return falcon::internal::detail::entry<Type>(server, plugin, callbacks); \
    }
