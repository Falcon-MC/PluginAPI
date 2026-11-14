#pragma once

#include "falcon/Api.hpp"
#include "falcon/BlockPos.hpp"
#include "falcon/Item.hpp"
#include "falcon/PlayerInventory.hpp"
#include "falcon/Vec3.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace falcon {
    class Entity;
    class Level;
    enum class Dimension : uint32_t;

    enum class CameraEase : uint32_t {
        Linear = FALCON_CAMERA_EASE_LINEAR,
        Spring = FALCON_CAMERA_EASE_SPRING,
        InSine = FALCON_CAMERA_EASE_IN_SINE,
        OutSine = FALCON_CAMERA_EASE_OUT_SINE,
        InOutSine = FALCON_CAMERA_EASE_IN_OUT_SINE,
        InQuad = FALCON_CAMERA_EASE_IN_QUAD,
        OutQuad = FALCON_CAMERA_EASE_OUT_QUAD,
        InOutQuad = FALCON_CAMERA_EASE_IN_OUT_QUAD,
        InCubic = FALCON_CAMERA_EASE_IN_CUBIC,
        OutCubic = FALCON_CAMERA_EASE_OUT_CUBIC,
        InOutCubic = FALCON_CAMERA_EASE_IN_OUT_CUBIC,
        InQuart = FALCON_CAMERA_EASE_IN_QUART,
        OutQuart = FALCON_CAMERA_EASE_OUT_QUART,
        InOutQuart = FALCON_CAMERA_EASE_IN_OUT_QUART,
        InQuint = FALCON_CAMERA_EASE_IN_QUINT,
        OutQuint = FALCON_CAMERA_EASE_OUT_QUINT,
        InOutQuint = FALCON_CAMERA_EASE_IN_OUT_QUINT,
        InExpo = FALCON_CAMERA_EASE_IN_EXPO,
        OutExpo = FALCON_CAMERA_EASE_OUT_EXPO,
        InOutExpo = FALCON_CAMERA_EASE_IN_OUT_EXPO,
        InCirc = FALCON_CAMERA_EASE_IN_CIRC,
        OutCirc = FALCON_CAMERA_EASE_OUT_CIRC,
        InOutCirc = FALCON_CAMERA_EASE_IN_OUT_CIRC,
        InBack = FALCON_CAMERA_EASE_IN_BACK,
        OutBack = FALCON_CAMERA_EASE_OUT_BACK,
        InOutBack = FALCON_CAMERA_EASE_IN_OUT_BACK,
        InElastic = FALCON_CAMERA_EASE_IN_ELASTIC,
        OutElastic = FALCON_CAMERA_EASE_OUT_ELASTIC,
        InOutElastic = FALCON_CAMERA_EASE_IN_OUT_ELASTIC,
        InBounce = FALCON_CAMERA_EASE_IN_BOUNCE,
        OutBounce = FALCON_CAMERA_EASE_OUT_BOUNCE,
        InOutBounce = FALCON_CAMERA_EASE_IN_OUT_BOUNCE
    };

    struct SkinImage {
        uint32_t width = 0;
        uint32_t height = 0;
        /** RGBA, four bytes a pixel, rows from the top. */
        std::vector<uint8_t> rgba;
    };

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

        /**
         * The network protocol of the player's game version, which can be older than the server's when it accepts
         * several versions.
         */
        int32_t protocolVersion() const {
            return detail::api().playerProtocolVersion(mHandle);
        }

        /**
         * Shows a form built from its JSON definition and returns its id, which the matching
         * PlayerFormResponseEvent carries. Returns 0 when the form could not be sent.
         */
        uint32_t sendForm(const std::string &formJson) const {
            return detail::api().playerSendForm(mHandle, formJson.c_str());
        }

        /**
         * Sends the player to another server. Returns false when the address is empty or the port invalid.
         */
        bool transfer(const std::string &address, uint32_t port = 19132) const {
            return detail::api().playerTransfer(mHandle, address.c_str(), port) != 0;
        }

        void sendTitle(const std::string &title, const std::string &subtitle = "") const {
            detail::api().playerSendTitle(mHandle, title.c_str(), subtitle.c_str());
        }

        /**
         * Shows a boss bar and returns its id. The newest bar of a player is displayed, the others wait behind it, and
         * a raid bar replaces them all while it is up. Percent goes from 0 to 1. Colors: 0 pink, 1 blue, 2 red,
         * 3 green, 4 yellow, 5 purple, 6 rebecca purple, 7 white.
         */
        uint32_t addBossBar(const std::string &title, float percent = 1.0f, uint32_t color = 5) const {
            return detail::api().playerAddBossBar(mHandle, title.c_str(), percent, color);
        }

        bool updateBossBar(uint32_t bar, const std::string &title, float percent, uint32_t color = 5) const {
            return detail::api().playerUpdateBossBar(mHandle, bar, title.c_str(), percent, color) != 0;
        }

        bool removeBossBar(uint32_t bar) const {
            return detail::api().playerRemoveBossBar(mHandle, bar) != 0;
        }

        /** Gives a filled image map of this player's world; dropped at the feet when the inventory is full. */
        bool giveMap(int64_t mapId) const {
            return detail::api().playerGiveMap(mHandle, mapId) != 0;
        }

        void sendActionBar(const std::string &message) const {
            detail::api().playerSendActionBar(mHandle, message.c_str());
        }

        /** The spawn point a bed, an anchor or /spawnpoint set; false when the player has none. */
        bool spawnPoint(BlockPos &position, Dimension &dimension) const {
            FalconBlockPos raw{};
            FalconDimension rawDimension = 0;
            if (detail::api().playerSpawnPoint(mHandle, &raw, &rawDimension) == 0)
                return false;
            position = BlockPos::from(raw);
            dimension = static_cast<Dimension>(rawDimension);
            return true;
        }

        bool setSpawnPoint(const Level &level, const BlockPos &position) const;

        void clearSpawnPoint() const {
            detail::api().playerClearSpawnPoint(mHandle);
        }

        /** The language code of the client, such as en_US. */
        std::string language() const {
            return detail::text(detail::api().playerLanguage(mHandle));
        }

        /** The platform number the client sent at login, in the game's own numbering. */
        int32_t deviceOs() const {
            return detail::api().playerDeviceOs(mHandle);
        }

        std::string skinId() const {
            return detail::text(detail::api().playerSkinId(mHandle));
        }

        SkinImage skinImage() const {
            SkinImage image;
            uint32_t length = 0;
            const uint8_t *data = detail::api().playerSkinImage(mHandle, &image.width, &image.height, &length);
            if (data != nullptr)
                image.rgba.assign(data, data + length);
            return image;
        }

        /** Switches the camera to a preset such as minecraft:free, easing over easeTime seconds when above 0. */
        bool setCamera(const std::string &preset, float easeTime = 0.0f, CameraEase ease = CameraEase::Linear) const {
            return detail::api().playerSetCamera(mHandle, preset.c_str(), nullptr, nullptr, easeTime,
                                                 static_cast<FalconCameraEase>(ease)) != 0;
        }

        /** A preset placed at a position and rotation (x pitch, y yaw, in degrees). */
        bool setCamera(const std::string &preset, const Vec3 &position, const Vec3 &rotation, float easeTime = 0.0f,
                       CameraEase ease = CameraEase::Linear) const {
            const FalconVec3 at = position.raw();
            const FalconVec3 turned = rotation.raw();
            return detail::api().playerSetCamera(mHandle, preset.c_str(), &at, &turned, easeTime,
                                                 static_cast<FalconCameraEase>(ease)) != 0;
        }

        /** Fades the screen to a 0xRRGGBB color and back, times in seconds. */
        bool fadeCamera(float fadeIn, float hold, float fadeOut, uint32_t color = 0x000000) const {
            return detail::api().playerFadeCamera(mHandle, fadeIn, hold, fadeOut, color) != 0;
        }

        void clearCamera() const {
            detail::api().playerClearCamera(mHandle);
        }

        /** Ticks left on the cooldown of the item's category, 0 when it is ready. */
        int32_t itemCooldown(const Item &item) const {
            return detail::api().playerItemCooldown(mHandle, item.handle());
        }

        bool startItemCooldown(const Item &item, int32_t ticks) const {
            return detail::api().playerStartItemCooldown(mHandle, item.handle(), ticks) != 0;
        }

        /** Runs a command line, without the slash, as this player, with the player's permissions. */
        bool executeCommand(const std::string &commandLine) const {
            return detail::api().playerExecuteCommand(mHandle, commandLine.c_str()) != 0;
        }

        /** Plays a sound only this player hears, from a position in its level. */
        void playSound(const std::string &sound, const Vec3 &position, float volume = 1.0f, float pitch = 1.0f) const {
            detail::api().playerPlaySound(mHandle, sound.c_str(), position.raw(), volume, pitch);
        }

        /** An empty name stops every sound. */
        void stopSound(const std::string &sound = "") const {
            detail::api().playerStopSound(mHandle, detail::nullable(sound));
        }

        /** Shows a particle effect only this player sees, in its level. */
        void spawnParticle(const std::string &particle, const Vec3 &position) const {
            detail::api().playerSpawnParticle(mHandle, particle.c_str(), position.raw());
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
#include "falcon/Level.hpp"

namespace falcon {
    inline Entity Player::entity() const {
        return Entity(detail::api().playerEntity(mHandle));
    }

    inline bool Player::setSpawnPoint(const Level &level, const BlockPos &position) const {
        return detail::api().playerSetSpawnPoint(mHandle, level.handle(), position.raw()) != 0;
    }
}
