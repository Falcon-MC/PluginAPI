#pragma once

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
    };
}
