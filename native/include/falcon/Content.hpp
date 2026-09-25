#pragma once

#include "falcon/Api.hpp"
#include "falcon/BlockPos.hpp"
#include "falcon/CustomBlock.hpp"
#include "falcon/CustomEntity.hpp"
#include "falcon/CustomItem.hpp"
#include "falcon/Entity.hpp"
#include "falcon/Item.hpp"
#include "falcon/Player.hpp"

#include <cstdint>
#include <memory>
#include <utility>

namespace falcon {
    class Content {
    public:
        bool item(CustomItem definition) const {
            auto *stored = detail::keep(std::make_unique<CustomItem>(std::move(definition)));

            FalconCustomItemDescriptor descriptor{};
            descriptor.identifier = stored->identifier.c_str();
            descriptor.displayName = detail::nullable(stored->displayName);
            descriptor.icon = detail::nullable(stored->icon);
            descriptor.creativeCategory = detail::nullable(stored->creativeCategory);
            descriptor.maxStackSize = stored->maxStackSize;
            descriptor.maxDurability = stored->maxDurability;
            descriptor.handEquipped = stored->handEquipped ? 1 : 0;
            descriptor.onUse = stored->onUse ? &useItem : nullptr;
            descriptor.onUseOnBlock = stored->onUseOnBlock ? &useItemOnBlock : nullptr;
            descriptor.userData = stored;
            return detail::api().registerCustomItem(detail::plugin(), &descriptor) != 0;
        }

        bool block(CustomBlock definition) const {
            auto *stored = detail::keep(std::make_unique<CustomBlock>(std::move(definition)));

            FalconCustomBlockDescriptor descriptor{};
            descriptor.identifier = stored->identifier.c_str();
            descriptor.displayName = detail::nullable(stored->displayName);
            descriptor.texture = detail::nullable(stored->texture);
            descriptor.creativeCategory = detail::nullable(stored->creativeCategory);
            descriptor.destroyTime = stored->destroyTime;
            descriptor.explosionResistance = stored->explosionResistance;
            descriptor.lightEmission = stored->lightEmission;
            descriptor.friction = stored->friction;
            descriptor.drop = detail::nullable(stored->drop);
            descriptor.onInteract = stored->onInteract ? &interactBlock : nullptr;
            descriptor.onBreak = stored->onBreak ? &breakBlock : nullptr;
            descriptor.userData = stored;
            return detail::api().registerCustomBlock(detail::plugin(), &descriptor) != 0;
        }

        bool entity(CustomEntity definition) const {
            auto *stored = detail::keep(std::make_unique<CustomEntity>(std::move(definition)));

            FalconCustomEntityDescriptor descriptor{};
            descriptor.identifier = stored->identifier.c_str();
            descriptor.width = stored->width;
            descriptor.height = stored->height;
            descriptor.maxHealth = stored->maxHealth;
            descriptor.summonable = stored->summonable ? 1 : 0;
            descriptor.onTick = stored->onTick ? &tickEntity : nullptr;
            descriptor.onInteract = stored->onInteract ? &interactEntity : nullptr;
            descriptor.userData = stored;
            return detail::api().registerCustomEntity(detail::plugin(), &descriptor) != 0;
        }

    private:
        static int useItem(FalconPlayer *player, FalconItem *item, void *userData) {
            int result = 0;
            detail::guarded("A custom item handler threw an exception", [&] {
                Player user(player);
                Item used = Item::borrow(item);
                result = static_cast<CustomItem *>(userData)->onUse(user, used) ? 1 : 0;
            });
            return result;
        }

        static int useItemOnBlock(FalconPlayer *player, FalconItem *item, FalconBlockPos position, uint32_t face,
                                  void *userData) {
            int result = 0;
            detail::guarded("A custom item handler threw an exception", [&] {
                Player user(player);
                Item used = Item::borrow(item);
                const BlockPos target = BlockPos::from(position);
                result = static_cast<CustomItem *>(userData)->onUseOnBlock(user, used, target, face) ? 1 : 0;
            });
            return result;
        }

        static int interactBlock(FalconPlayer *player, FalconBlockPos position, uint32_t face, void *userData) {
            int result = 0;
            detail::guarded("A custom block handler threw an exception", [&] {
                Player user(player);
                const BlockPos target = BlockPos::from(position);
                result = static_cast<CustomBlock *>(userData)->onInteract(user, target, face) ? 1 : 0;
            });
            return result;
        }

        static void breakBlock(FalconPlayer *player, FalconBlockPos position, void *userData) {
            detail::guarded("A custom block handler threw an exception", [&] {
                Player user(player);
                const BlockPos target = BlockPos::from(position);
                static_cast<CustomBlock *>(userData)->onBreak(user, target);
            });
        }

        static void tickEntity(FalconEntity *entity, void *userData) {
            detail::guarded("A custom entity handler threw an exception", [&] {
                Entity ticked(entity);
                static_cast<CustomEntity *>(userData)->onTick(ticked);
            });
        }

        static int interactEntity(FalconEntity *entity, FalconPlayer *player, void *userData) {
            int result = 0;
            detail::guarded("A custom entity handler threw an exception", [&] {
                Entity target(entity);
                Player user(player);
                result = static_cast<CustomEntity *>(userData)->onInteract(target, user) ? 1 : 0;
            });
            return result;
        }
    };
}
