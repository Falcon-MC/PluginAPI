#pragma once

#include "falcon/Api.hpp"
#include "falcon/PlayerInventory.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace falcon {
    class Entity;

    enum class GameMode : uint32_t {
        Survival = FALCON_GAME_MODE_SURVIVAL,
        Creative = FALCON_GAME_MODE_CREATIVE,
        Adventure = FALCON_GAME_MODE_ADVENTURE,
        Spectator = FALCON_GAME_MODE_SPECTATOR
    };

    class Player {
    public:
        explicit Player(FalconPlayer *handle) : mHandle(handle) {
        }

        bool valid() const {
            return mHandle != nullptr;
        }

        std::string name() const {
            return detail::text(detail::api().playerName(mHandle));
        }

        bool isOperator() const {
            return detail::api().playerIsOperator(mHandle) != 0;
        }

        void setOperator(bool value) const {
            detail::api().playerSetOperator(mHandle, value ? 1 : 0);
        }

        void sendMessage(const std::string &message) const {
            detail::api().playerSendMessage(mHandle, message.c_str());
        }

        void kick(const std::string &reason) const {
            detail::api().playerKick(mHandle, reason.c_str());
        }

        Entity entity() const;

        GameMode gameMode() const {
            return static_cast<GameMode>(detail::api().playerGameMode(mHandle));
        }

        void setGameMode(GameMode gameMode) const {
            detail::api().playerSetGameMode(mHandle, static_cast<FalconGameMode>(gameMode));
        }

        std::string xuid() const {
            return detail::text(detail::api().playerXuid(mHandle));
        }

        std::string uuid() const {
            return detail::text(detail::api().playerUuid(mHandle));
        }

        std::string address() const {
            return detail::text(detail::api().playerAddress(mHandle));
        }

        void sendTitle(const std::string &title, const std::string &subtitle = "") const {
            detail::api().playerSendTitle(mHandle, title.c_str(), subtitle.c_str());
        }

        void sendActionBar(const std::string &message) const {
            detail::api().playerSendActionBar(mHandle, message.c_str());
        }

        float food() const {
            return detail::api().playerFood(mHandle);
        }

        void setFood(float food) const {
            detail::api().playerSetFood(mHandle, food);
        }

        int32_t xpLevel() const {
            return detail::api().playerXpLevel(mHandle);
        }

        void setXpLevel(int32_t level) const {
            detail::api().playerSetXpLevel(mHandle, level);
        }

        PlayerInventory inventory() const {
            return PlayerInventory(mHandle);
        }

        bool hasPermission(const std::string &node) const {
            return detail::api().playerHasPermission(mHandle, node.c_str()) != 0;
        }

        void setPermission(const std::string &node, bool value = true) const {
            detail::api().playerSetPermission(detail::plugin(), mHandle, node.c_str(), value ? 1 : 0);
        }

        void unsetPermission(const std::string &node) const {
            detail::api().playerUnsetPermission(detail::plugin(), mHandle, node.c_str());
        }

        bool sendPacket(uint32_t packetId, const uint8_t *data, std::size_t length) const {
            return detail::api().playerSendPacket(mHandle, packetId, data, static_cast<uint32_t>(length)) != 0;
        }

        bool sendPacket(uint32_t packetId, const std::vector<uint8_t> &data) const {
            return sendPacket(packetId, data.data(), data.size());
        }

        bool sendPacket(uint32_t packetId, std::string_view data) const {
            return sendPacket(packetId, reinterpret_cast<const uint8_t *>(data.data()), data.size());
        }

        FalconPlayer *handle() const {
            return mHandle;
        }

    private:
        FalconPlayer *mHandle;
    };
}

#include "falcon/Entity.hpp"

namespace falcon {
    inline Entity Player::entity() const {
        return Entity(detail::api().playerEntity(mHandle));
    }
}
