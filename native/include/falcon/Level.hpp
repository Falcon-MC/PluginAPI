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

    enum class BlockFace : uint32_t {
        Down = FALCON_FACE_DOWN,
        Up = FALCON_FACE_UP,
        North = FALCON_FACE_NORTH,
        South = FALCON_FACE_SOUTH,
        West = FALCON_FACE_WEST,
        East = FALCON_FACE_EAST
    };

    enum class Mirror : uint32_t {
        None = FALCON_MIRROR_NONE,
        X = FALCON_MIRROR_X,
        Z = FALCON_MIRROR_Z,
        XZ = FALCON_MIRROR_XZ
    };

    struct BlockHit {
        BlockPos block;
        BlockFace face = BlockFace::Up;
        /** Where the ray enters the block. */
        Vec3 position;
        double distance = 0.0;
    };

    struct EntityHit;

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

        std::vector<std::string> gameRules() const {
            std::vector<std::string> names;
            const uint32_t count = detail::api().levelGameRuleCount(mHandle);
            for (uint32_t index = 0; index < count; ++index)
                names.push_back(detail::text(detail::api().levelGameRuleName(mHandle, index)));
            return names;
        }

        /** "true" or "false" for a switch, the number otherwise; the rules belong to the level's world. */
        std::optional<std::string> gameRule(const std::string &name) const {
            const char *value = detail::api().levelGameRule(mHandle, name.c_str());
            if (value == nullptr)
                return std::nullopt;
            return std::string(value);
        }

        bool setGameRule(const std::string &name, const std::string &value) const {
            return detail::api().levelSetGameRule(mHandle, name.c_str(), value.c_str()) != 0;
        }

        /** The biome identifier at a position, such as minecraft:plains. */
        std::string biome(const BlockPos &position) const {
            return detail::text(detail::api().levelBiome(mHandle, position.raw()));
        }

        /** The first block on the ray; an unloaded chunk stops it without a hit. */
        std::optional<BlockHit> raycastBlock(const Vec3 &origin, const Vec3 &direction, double maxDistance,
                                             bool includeLiquids = false, bool includePassable = false) const {
            FalconBlockHit hit{};
            if (detail::api().levelRaycastBlock(mHandle, origin.raw(), direction.raw(), maxDistance,
                                                includeLiquids ? 1 : 0, includePassable ? 1 : 0, &hit) == 0)
                return std::nullopt;
            return BlockHit{BlockPos::from(hit.block), static_cast<BlockFace>(hit.face), Vec3::from(hit.position),
                            hit.distance};
        }

        /** The nearest entity on the ray, before any block; except is left out, usually the one looking. */
        std::optional<EntityHit> raycastEntity(const Vec3 &origin, const Vec3 &direction, double maxDistance) const;

        std::optional<EntityHit> raycastEntity(const Vec3 &origin, const Vec3 &direction, double maxDistance,
                                               const Entity &except) const;

        /** Every entity and player whose box touches the box between two corners. */
        std::vector<Entity> entitiesInBox(const Vec3 &from, const Vec3 &to) const;

        /**
         * Places a structure saved by the structure manager, a pack or a structure block, by identifier, with its
         * lowest corner at position. Rotation is in quarter turns clockwise; integrity keeps that share of blocks.
         */
        bool placeStructure(const std::string &structure, const BlockPos &position, uint32_t rotation = 0,
                            Mirror mirror = Mirror::None, bool includeEntities = true, float integrity = 1.0f) const {
            return detail::api().levelPlaceStructure(mHandle, structure.c_str(), position.raw(), rotation,
                                                     static_cast<FalconMirror>(mirror), includeEntities ? 1 : 0,
                                                     integrity) != 0;
        }

        /** Keeps the chunks between two corners loaded and ticking, with the limits of /tickingarea. */
        bool addTickingArea(const std::string &name, const BlockPos &from, const BlockPos &to,
                            bool preload = false) const {
            return detail::api().levelAddTickingArea(mHandle, name.c_str(), from.raw(), to.raw(), preload ? 1 : 0) != 0;
        }

        bool removeTickingArea(const std::string &name) const {
            return detail::api().levelRemoveTickingArea(mHandle, name.c_str()) != 0;
        }

        std::vector<std::string> tickingAreas() const {
            std::vector<std::string> names;
            const uint32_t count = detail::api().levelTickingAreaCount(mHandle);
            for (uint32_t index = 0; index < count; ++index)
                names.push_back(detail::text(detail::api().levelTickingAreaName(mHandle, index)));
            return names;
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
    struct EntityHit {
        Entity entity;
        double distance = 0.0;
    };

    inline std::optional<EntityHit> Level::raycastEntity(const Vec3 &origin, const Vec3 &direction,
                                                         double maxDistance) const {
        double distance = 0.0;
        FalconEntity *hit = detail::api().levelRaycastEntity(mHandle, origin.raw(), direction.raw(), maxDistance,
                                                             nullptr, &distance);
        if (hit == nullptr)
            return std::nullopt;
        return EntityHit{Entity(hit), distance};
    }

    inline std::optional<EntityHit> Level::raycastEntity(const Vec3 &origin, const Vec3 &direction, double maxDistance,
                                                         const Entity &except) const {
        double distance = 0.0;
        FalconEntity *hit = detail::api().levelRaycastEntity(mHandle, origin.raw(), direction.raw(), maxDistance,
                                                             except.handle(), &distance);
        if (hit == nullptr)
            return std::nullopt;
        return EntityHit{Entity(hit), distance};
    }

    inline std::vector<Entity> Level::entitiesInBox(const Vec3 &from, const Vec3 &to) const {
        std::vector<FalconEntity *> handles(64);
        uint32_t count = detail::api().levelEntitiesInBox(mHandle, from.raw(), to.raw(), handles.data(),
                                                          static_cast<uint32_t>(handles.size()));
        if (count > handles.size()) {
            handles.resize(count);
            count = detail::api().levelEntitiesInBox(mHandle, from.raw(), to.raw(), handles.data(), count);
        }

        std::vector<Entity> result;
        for (uint32_t index = 0; index < count && index < handles.size(); ++index)
            result.emplace_back(handles[index]);
        return result;
    }

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
