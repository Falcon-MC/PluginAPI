#pragma once

#include "falcon/Api.hpp"
#include "falcon/Block.hpp"
#include "falcon/BlockPos.hpp"
#include "falcon/Item.hpp"
#include "falcon/Vec3.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace falcon {
    class Entity;

    enum class SignSide : uint32_t {
        Front = FALCON_SIGN_FRONT,
        Back = FALCON_SIGN_BACK
    };

    enum class Dimension : uint32_t {
        Overworld = FALCON_DIMENSION_OVERWORLD,
        Nether = FALCON_DIMENSION_NETHER,
        TheEnd = FALCON_DIMENSION_THE_END
    };

    class Level {
    public:
        explicit Level(FalconLevel *handle) : mHandle(handle) {
        }

        bool valid() const {
            return mHandle != nullptr;
        }

        Dimension dimension() const {
            return static_cast<Dimension>(detail::api().levelDimension(mHandle));
        }

        std::string name() const {
            return detail::text(detail::api().levelName(mHandle));
        }

        Block getBlock(const BlockPos &position) const {
            return Block{position, blockName(position), blockStates(position)};
        }

        std::string blockName(const BlockPos &position) const {
            return detail::text(detail::api().levelGetBlock(mHandle, position.raw()));
        }

        std::string blockStates(const BlockPos &position) const {
            return detail::text(detail::api().levelGetBlockStates(mHandle, position.raw()));
        }

        bool setBlock(const BlockPos &position, const std::string &name, const std::string &statesJson = "") const {
            const char *states = detail::nullable(statesJson);
            return detail::api().levelSetBlock(mHandle, position.raw(), name.c_str(), states) != 0;
        }

        bool breakBlock(const BlockPos &position, bool dropItems = true) const {
            return detail::api().levelBreakBlock(mHandle, position.raw(), dropItems ? 1 : 0) != 0;
        }

        bool isChunkLoaded(int32_t chunkX, int32_t chunkZ) const {
            return detail::api().levelIsChunkLoaded(mHandle, chunkX, chunkZ) != 0;
        }

        int32_t highestBlockY(int32_t x, int32_t z) const {
            return detail::api().levelHighestBlockY(mHandle, x, z);
        }

        int64_t time() const {
            return detail::api().levelTime(mHandle);
        }

        void setTime(int64_t time) const {
            detail::api().levelSetTime(mHandle, time);
        }

        bool isRaining() const {
            return detail::api().levelIsRaining(mHandle) != 0;
        }

        void setRaining(bool raining) const {
            detail::api().levelSetRaining(mHandle, raining ? 1 : 0);
        }

        bool isThundering() const {
            return detail::api().levelIsThundering(mHandle) != 0;
        }

        void setThundering(bool thundering) const {
            detail::api().levelSetThundering(mHandle, thundering ? 1 : 0);
        }

        Vec3 spawnPosition() const {
            return Vec3::from(detail::api().levelSpawnPosition(mHandle));
        }

        std::optional<Entity> spawnEntity(const std::string &identifier, const Vec3 &position) const;

        void dropItem(const Vec3 &position, const Item &item) const {
            detail::api().levelDropItem(mHandle, position.raw(), item.handle());
        }

        void strikeLightning(const Vec3 &position) const {
            detail::api().levelStrikeLightning(mHandle, position.raw());
        }

        void createExplosion(const Vec3 &position, float power, bool breakBlocks = true) const {
            detail::api().levelCreateExplosion(mHandle, position.raw(), power, breakBlocks ? 1 : 0);
        }

        /** Plays a sound for every player in this level; the client fades it with distance. */
        void playSound(const std::string &sound, const Vec3 &position, float volume = 1.0f, float pitch = 1.0f) const {
            detail::api().levelPlaySound(mHandle, sound.c_str(), position.raw(), volume, pitch);
        }

        void spawnParticle(const std::string &particle, const Vec3 &position) const {
            detail::api().levelSpawnParticle(mHandle, particle.c_str(), position.raw());
        }

        /** The slot count of the chest, barrel, furnace, hopper or other container block there; 0 for none. */
        uint32_t containerSize(const BlockPos &position) const {
            return detail::api().levelContainerSize(mHandle, position.raw());
        }

        /** A copy of the item in a container slot; nothing for an empty slot or no container. */
        std::optional<Item> containerItem(const BlockPos &position, uint32_t slot) const {
            std::optional<Item> found = Item::adopt(detail::api().levelContainerItem(mHandle, position.raw(), slot));
            if (found.has_value() && found->isEmpty())
                return std::nullopt;
            return found;
        }

        bool setContainerItem(const BlockPos &position, uint32_t slot, const Item &item) const {
            return detail::api().levelSetContainerItem(mHandle, position.raw(), slot, item.handle()) != 0;
        }

        bool clearContainerItem(const BlockPos &position, uint32_t slot) const {
            return detail::api().levelSetContainerItem(mHandle, position.raw(), slot, nullptr) != 0;
        }

        /** The text of a sign or hanging sign, raw or JSON as the game saved it; nothing when no sign is there. */
        std::optional<std::string> signText(const BlockPos &position, SignSide side = SignSide::Front) const {
            const char *text = detail::api().levelSignText(mHandle, position.raw(), static_cast<FalconSignSide>(side));
            if (text == nullptr)
                return std::nullopt;
            return std::string(text);
        }

        bool setSignText(const BlockPos &position, const std::string &text, SignSide side = SignSide::Front) const {
            return detail::api().levelSetSignText(mHandle, position.raw(), static_cast<FalconSignSide>(side),
                                                  text.c_str()) != 0;
        }

        std::vector<Entity> entities() const;

        FalconLevel *handle() const {
            return mHandle;
        }

    private:
        FalconLevel *mHandle;
    };
}

#include "falcon/Entity.hpp"

namespace falcon {
    inline std::optional<Entity> Level::spawnEntity(const std::string &identifier, const Vec3 &position) const {
        FalconEntity *entity = detail::api().levelSpawnEntity(mHandle, identifier.c_str(), position.raw());
        if (entity == nullptr)
            return std::nullopt;
        return Entity(entity);
    }

    inline std::vector<Entity> Level::entities() const {
        std::vector<Entity> result;
        const uint32_t count = detail::api().levelEntityCount(mHandle);
        result.reserve(count);
        for (uint32_t index = 0; index < count; index++)
            result.emplace_back(detail::api().levelEntity(mHandle, index));
        return result;
    }
}
