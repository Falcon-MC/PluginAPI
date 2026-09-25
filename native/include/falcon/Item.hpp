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
