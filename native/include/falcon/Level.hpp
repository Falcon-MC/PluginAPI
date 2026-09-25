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
