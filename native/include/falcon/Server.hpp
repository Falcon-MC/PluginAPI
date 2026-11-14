#pragma once

#include "falcon/Level.hpp"
#include "falcon/Player.hpp"

#include <optional>
#include <string>
#include <vector>

namespace falcon {
    enum class Difficulty : uint32_t {
        Peaceful = FALCON_DIFFICULTY_PEACEFUL,
        Easy = FALCON_DIFFICULTY_EASY,
        Normal = FALCON_DIFFICULTY_NORMAL,
        Hard = FALCON_DIFFICULTY_HARD
    };

    class Server {
    public:
        static Difficulty difficulty() {
            return static_cast<Difficulty>(detail::api().serverDifficulty());
        }

        /** Saved in the server properties and sent to every player, like /difficulty. */
        static bool setDifficulty(Difficulty difficulty) {
            return detail::api().serverSetDifficulty(static_cast<FalconDifficulty>(difficulty)) != 0;
        }

        /** Bans a name, like /ban, and kicks the player when online; the reason is shown to them. */
        static bool ban(const std::string &name, const std::string &reason = "") {
            const char *source = detail::api().pluginName(detail::plugin());
            return detail::api().serverBan(name.c_str(), reason.c_str(), source) != 0;
        }

        static bool pardon(const std::string &name) {
            return detail::api().serverPardon(name.c_str()) != 0;
        }

        static bool isBanned(const std::string &name) {
            return detail::api().serverIsBanned(name.c_str()) != 0;
        }

        static std::vector<std::string> bannedNames() {
            return list(detail::api().serverBanCount(), detail::api().serverBannedName);
        }

        /** Bans an address, like /ban-ip, and kicks every player connected from it. */
        static bool banIp(const std::string &address, const std::string &reason = "") {
            return detail::api().serverBanIp(address.c_str(), reason.c_str(),
                                             detail::api().pluginName(detail::plugin())) != 0;
        }

        static bool pardonIp(const std::string &address) {
            return detail::api().serverPardonIp(address.c_str()) != 0;
        }

        static bool isIpBanned(const std::string &address) {
            return detail::api().serverIsIpBanned(address.c_str()) != 0;
        }

        static std::vector<std::string> bannedIps() {
            return list(detail::api().serverIpBanCount(), detail::api().serverBannedIp);
        }

        static bool allowListAdd(const std::string &name) {
            return detail::api().serverAllowListAdd(name.c_str()) != 0;
        }

        /** Removes a name and kicks the players the allow list no longer lets in. */
        static bool allowListRemove(const std::string &name) {
            return detail::api().serverAllowListRemove(name.c_str()) != 0;
        }

        static std::vector<std::string> allowList() {
            return list(detail::api().serverAllowListCount(), detail::api().serverAllowListName);
        }

        static bool allowListEnabled() {
            return detail::api().serverAllowListEnabled() != 0;
        }

        static void setAllowListEnabled(bool enabled) {
            detail::api().serverSetAllowListEnabled(enabled ? 1 : 0);
        }

        static std::string motd() {
            return detail::text(detail::api().serverMotd());
        }

        static std::string subMotd() {
            return detail::text(detail::api().serverSubMotd());
        }

        /** The two lines of the server list entry, until the server restarts. */
        static void setMotd(const std::string &motd, const std::string &subMotd = "") {
            detail::api().serverSetMotd(motd.c_str(), subMotd.c_str());
        }

        static uint32_t maxPlayers() {
            return detail::api().serverMaxPlayers();
        }

        static bool setMaxPlayers(uint32_t maxPlayers) {
            return detail::api().serverSetMaxPlayers(maxPlayers) != 0;
        }

        /** Ticks per second over the last ticks, at most 20. */
        static double tps() {
            return detail::api().serverTps();
        }

        static double millisecondsPerTick() {
            return detail::api().serverMillisecondsPerTick();
        }

        /** Stops the server after the current tick, like /stop. */
        static void shutdown() {
            detail::api().serverShutdown();
        }

        static std::vector<std::string> plugins() {
            return list(detail::api().serverPluginCount(), detail::api().serverPluginName);
        }

        static std::optional<std::string> pluginVersion(const std::string &name) {
            const char *version = detail::api().serverPluginVersion(name.c_str());
            if (version == nullptr)
                return std::nullopt;
            return std::string(version);
        }

        static bool isPluginEnabled(const std::string &name) {
            return detail::api().serverPluginEnabled(name.c_str()) != 0;
        }

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

    private:
        static std::vector<std::string> list(uint32_t count, const char *(*at)(uint32_t)) {
            std::vector<std::string> values;
            for (uint32_t index = 0; index < count; ++index)
                values.push_back(detail::text(at(index)));
            return values;
        }
    };
}
