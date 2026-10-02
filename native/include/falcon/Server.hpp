#pragma once

#include "falcon/Level.hpp"
#include "falcon/Player.hpp"

#include <optional>
#include <string>
#include <vector>

namespace falcon {
    class Server {
    public:
        static std::string version() {
            return detail::api().serverVersion();
        }

        static std::vector<Player> onlinePlayers() {
            std::vector<Player> players;
            const uint32_t count = detail::api().onlinePlayerCount();
            for (uint32_t index = 0; index < count; index++)
                players.emplace_back(detail::api().onlinePlayer(index));
            return players;
        }

        static std::optional<Player> findPlayer(const std::string &name) {
            FalconPlayer *handle = detail::api().findPlayer(name.c_str());
            if (handle == nullptr)
                return std::nullopt;
            return Player(handle);
        }

        static void broadcast(const std::string &message) {
            detail::api().broadcastMessage(message.c_str());
        }

        static std::optional<Level> level(Dimension dimension) {
            FalconLevel *handle = detail::api().serverLevel(static_cast<FalconDimension>(dimension));
            if (handle == nullptr)
                return std::nullopt;
            return Level(handle);
        }

        /** A dimension of a loaded world, or nothing when no world has that name. */
        static std::optional<Level> level(const std::string &world, Dimension dimension) {
            FalconLevel *handle = detail::api().worldLevel(world.c_str(), static_cast<FalconDimension>(dimension));
            if (handle == nullptr)
                return std::nullopt;
            return Level(handle);
        }

        static std::vector<std::string> worlds() {
            std::vector<std::string> names;
            const uint32_t count = detail::api().serverWorldCount();
            for (uint32_t index = 0; index < count; index++)
                names.push_back(detail::text(detail::api().serverWorldName(index)));
            return names;
        }

        static std::string defaultWorld() {
            return detail::text(detail::api().serverDefaultWorldName());
        }

        /** Loads a world from disk, creating it when `create` is set. An empty seed picks a random one. */
        static bool loadWorld(const std::string &name, bool create = false, const std::string &seed = std::string()) {
            return detail::api().serverLoadWorld(name.c_str(), create ? 1 : 0,
                                                 seed.empty() ? nullptr : seed.c_str()) != 0;
        }

        /** Saves and closes a world; fails for the default world and for worlds that still hold players. */
        static bool unloadWorld(const std::string &name) {
            return detail::api().serverUnloadWorld(name.c_str()) != 0;
        }
    };
}
