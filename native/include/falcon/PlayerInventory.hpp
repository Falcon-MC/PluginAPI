#pragma once

#include "falcon/Api.hpp"
#include "falcon/Item.hpp"

#include <cstdint>
#include <optional>

namespace falcon {
    enum class ArmorSlot : uint32_t {
        Head = FALCON_ARMOR_HEAD,
        Chest = FALCON_ARMOR_CHEST,
        Legs = FALCON_ARMOR_LEGS,
        Feet = FALCON_ARMOR_FEET
    };

    class PlayerInventory {
    public:
        explicit PlayerInventory(FalconPlayer *player) : mPlayer(player) {
        }

        uint32_t size() const {
            return detail::api().playerInventorySize(mPlayer);
        }

        std::optional<Item> item(uint32_t slot) const {
            return Item::adopt(detail::api().playerInventoryItem(mPlayer, slot));
        }

        void setItem(uint32_t slot, const Item &item) const {
            detail::api().playerSetInventoryItem(mPlayer, slot, item.handle());
        }

        bool give(const Item &item) const {
            return detail::api().playerGiveItem(mPlayer, item.handle()) != 0;
        }

        uint32_t enderChestSize() const {
            return detail::api().playerEnderChestSize(mPlayer);
        }

        /** The player's own ender chest, the same in every ender chest block and saved with the player. */
        std::optional<Item> enderChestItem(uint32_t slot) const {
            std::optional<Item> found = Item::adopt(detail::api().playerEnderChestItem(mPlayer, slot));
            if (found.has_value() && found->isEmpty())
                return std::nullopt;
            return found;
        }

        bool setEnderChestItem(uint32_t slot, const Item &item) const {
            return detail::api().playerSetEnderChestItem(mPlayer, slot, item.handle()) != 0;
        }

        bool clearEnderChestItem(uint32_t slot) const {
            return detail::api().playerSetEnderChestItem(mPlayer, slot, nullptr) != 0;
        }

        uint32_t selectedSlot() const {
            return detail::api().playerSelectedSlot(mPlayer);
        }

        void setSelectedSlot(uint32_t slot) const {
            detail::api().playerSetSelectedSlot(mPlayer, slot);
        }

        std::optional<Item> heldItem() const {
            return item(selectedSlot());
        }

        std::optional<Item> armor(ArmorSlot slot) const {
            return Item::adopt(detail::api().playerArmorItem(mPlayer, static_cast<FalconArmorSlot>(slot)));
        }

        void setArmor(ArmorSlot slot, const Item &item) const {
            detail::api().playerSetArmorItem(mPlayer, static_cast<FalconArmorSlot>(slot), item.handle());
        }

        std::optional<Item> offhand() const {
            return Item::adopt(detail::api().playerOffhandItem(mPlayer));
        }

        void setOffhand(const Item &item) const {
            detail::api().playerSetOffhandItem(mPlayer, item.handle());
        }

        void clear() const {
            detail::api().playerClearInventory(mPlayer);
        }

    private:
        FalconPlayer *mPlayer;
    };
}
