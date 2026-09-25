#pragma once

#include "falcon/BlockPos.hpp"
#include "falcon/Entity.hpp"
#include "falcon/Events.hpp"
#include "falcon/Level.hpp"
#include "falcon/Player.hpp"
#include "falcon/Vec3.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace falcon {
    class ProjectileHitEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_PROJECTILE_HIT;

        using Event::Event;

        Entity projectile() const {
            return Entity(detail::api().eventEntity(mHandle));
        }

        std::optional<Entity> shooter() const {
            FalconEntity *source = detail::api().eventAttacker(mHandle);
            if (source == nullptr)
                return std::nullopt;
            return Entity(source);
        }

        std::optional<Entity> target() const {
            FalconEntity *hit = detail::api().eventTarget(mHandle);
            if (hit == nullptr)
                return std::nullopt;
            return Entity(hit);
        }

        bool hitBlock() const {
            return !detail::text(detail::api().eventBlockName(mHandle)).empty();
        }

        BlockPos blockPosition() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        std::string blockName() const {
            return detail::text(detail::api().eventBlockName(mHandle));
        }

        Vec3 position() const {
            return Vec3::from(detail::api().eventPosition(mHandle));
        }

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }
    };

    class ExplosionEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_EXPLOSION;

        using Event::Event;

        std::optional<Entity> source() const {
            FalconEntity *entity = detail::api().eventEntity(mHandle);
            if (entity == nullptr)
                return std::nullopt;
            return Entity(entity);
        }

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }

        Vec3 position() const {
            return Vec3::from(detail::api().eventPosition(mHandle));
        }

        double power() const {
            return detail::api().eventAmount(mHandle);
        }

        void setPower(double power) const {
            detail::api().eventSetAmount(mHandle, power);
        }

        std::vector<BlockPos> blocks() const {
            const uint32_t count = detail::api().eventBlockCount(mHandle);
            std::vector<BlockPos> result;
            result.reserve(count);
            for (uint32_t index = 0; index < count; ++index) {
                result.push_back(BlockPos::from(detail::api().eventBlockAt(mHandle, index)));
            }
            return result;
        }

        void setBlocks(const std::vector<BlockPos> &blocks) const {
            std::vector<FalconBlockPos> raw;
            raw.reserve(blocks.size());
            for (const BlockPos &block : blocks) {
                raw.push_back(block.raw());
            }
            detail::api().eventSetBlocks(mHandle, raw.data(), (uint32_t) raw.size());
        }
    };

    class FireSpreadEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_FIRE_SPREAD;

        using Event::Event;

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        Vec3 source() const {
            return Vec3::from(detail::api().eventPosition(mHandle));
        }
    };

    class BlockBurnEvent : public Event {
    public:
        static constexpr FalconEventType TYPE = FALCON_EVENT_BLOCK_BURN;

        using Event::Event;

        Level level() const {
            return Level(detail::api().eventLevel(mHandle));
        }

        BlockPos position() const {
            return BlockPos::from(detail::api().eventBlockPosition(mHandle));
        }

        std::string blockName() const {
            return detail::text(detail::api().eventBlockName(mHandle));
        }

        Vec3 source() const {
            return Vec3::from(detail::api().eventPosition(mHandle));
        }
    };
}
