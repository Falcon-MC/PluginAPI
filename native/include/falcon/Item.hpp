#pragma once

#include "falcon/Api.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace falcon {
    class Item {
    public:
        static std::optional<Item> create(const std::string &identifier, uint32_t count = 1) {
            return adopt(detail::api().itemCreate(identifier.c_str(), count));
        }

        static std::optional<Item> adopt(FalconItem *handle) {
            if (handle == nullptr)
                return std::nullopt;
            return Item(handle, true);
        }

        static Item borrow(FalconItem *handle) {
            return Item(handle, false);
        }

        Item(const Item &other) : mHandle(copyOf(other.mHandle)), mOwned(mHandle != nullptr) {
        }

        Item(Item &&other) noexcept : mHandle(other.mHandle), mOwned(other.mOwned) {
            other.mHandle = nullptr;
            other.mOwned = false;
        }

        Item &operator=(Item other) noexcept {
            std::swap(mHandle, other.mHandle);
            std::swap(mOwned, other.mOwned);
            return *this;
        }

        ~Item() {
            if (mOwned && mHandle != nullptr)
                detail::api().itemDestroy(mHandle);
        }

        bool valid() const {
            return mHandle != nullptr;
        }

        bool owned() const {
            return mOwned;
        }

        bool isEmpty() const {
            return mHandle == nullptr || detail::api().itemIsEmpty(mHandle) != 0;
        }

        std::string identifier() const {
            return detail::text(detail::api().itemIdentifier(mHandle));
        }

        uint32_t count() const {
            return detail::api().itemCount(mHandle);
        }

        void setCount(uint32_t count) const {
            detail::api().itemSetCount(mHandle, count);
        }

        int32_t damage() const {
            return detail::api().itemDamage(mHandle);
        }

        void setDamage(int32_t damage) const {
            detail::api().itemSetDamage(mHandle, damage);
        }

        std::string customName() const {
            return detail::text(detail::api().itemCustomName(mHandle));
        }

        void setCustomName(const std::string &name) const {
            detail::api().itemSetCustomName(mHandle, name.c_str());
        }

        std::vector<std::string> lore() const {
            std::vector<std::string> lines;
            const uint32_t count = detail::api().itemLoreCount(mHandle);
            lines.reserve(count);
            for (uint32_t index = 0; index < count; index++)
                lines.push_back(detail::text(detail::api().itemLore(mHandle, index)));
            return lines;
        }

        void setLore(const std::vector<std::string> &lines) const {
            std::vector<const char *> values;
            values.reserve(lines.size());
            for (const std::string &line: lines)
                values.push_back(line.c_str());
            detail::api().itemSetLore(mHandle, values.data(), static_cast<uint32_t>(values.size()));
        }

        int32_t enchantmentLevel(uint32_t enchantment) const {
            return detail::api().itemEnchantmentLevel(mHandle, enchantment);
        }

        void setEnchantmentLevel(uint32_t enchantment, int32_t level) const {
            detail::api().itemSetEnchantmentLevel(mHandle, enchantment, level);
        }

        /**
         * Text this plugin keeps on the item, saved with the item wherever it goes. Keys belong to the plugin: another
         * plugin never sees them. A copy of an inventory item must be put back for the change to stay.
         */
        std::optional<std::string> data(const std::string &key) const {
            const char *value = detail::api().itemData(detail::plugin(), mHandle, key.c_str());
            if (value == nullptr)
                return std::nullopt;
            return std::string(value);
        }

        bool setData(const std::string &key, const std::string &value) const {
            return detail::api().itemSetData(detail::plugin(), mHandle, key.c_str(), value.c_str()) != 0;
        }

        bool removeData(const std::string &key) const {
            return detail::api().itemSetData(detail::plugin(), mHandle, key.c_str(), nullptr) != 0;
        }

        std::vector<std::string> dataKeys() const {
            std::vector<std::string> keys;
            const uint32_t count = detail::api().itemDataCount(detail::plugin(), mHandle);
            for (uint32_t index = 0; index < count; index++)
                keys.push_back(detail::text(detail::api().itemDataKey(detail::plugin(), mHandle, index)));
            return keys;
        }

        /** The item's tag as little-endian NBT, the game's save format; empty for an item without one. */
        std::vector<uint8_t> nbt() const {
            uint32_t length = 0;
            const uint8_t *data = detail::api().itemNbt(mHandle, &length);
            if (data == nullptr)
                return {};
            return std::vector<uint8_t>(data, data + length);
        }

        /** Replaces the item's tag; empty data removes it. False when the data is not a little-endian NBT compound. */
        bool setNbt(const std::vector<uint8_t> &data) const {
            return detail::api().itemSetNbt(mHandle, data.data(), static_cast<uint32_t>(data.size())) != 0;
        }

        FalconItem *handle() const {
            return mHandle;
        }

    private:
        Item(FalconItem *handle, bool owned) : mHandle(handle), mOwned(owned) {
        }

        static FalconItem *copyOf(FalconItem *handle) {
            if (handle == nullptr)
                return nullptr;
            return detail::api().itemCopy(handle);
        }

        FalconItem *mHandle;
        bool mOwned;
    };
}
