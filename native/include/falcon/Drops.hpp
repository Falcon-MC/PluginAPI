#pragma once

#include "falcon/Events.hpp"
#include "falcon/Item.hpp"

#include <cstdint>
#include <vector>

namespace falcon {
    /** What an event drops: the items and the experience, which a handler may replace. */
    class DropsEvent : public Event {
    public:
        using Event::Event;

        /** Copies of the items that will drop. */
        std::vector<Item> drops() const {
            std::vector<Item> items;
            const uint32_t count = detail::api().eventDropCount(mHandle);
            items.reserve(count);
            for (uint32_t index = 0; index < count; ++index) {
                FalconItem *item = detail::api().eventDropAt(mHandle, index);
                if (item != nullptr)
                    items.push_back(Item(Item::borrow(item)));
            }
            return items;
        }

        void setDrops(const std::vector<Item> &items) const {
            std::vector<FalconItem *> handles;
            handles.reserve(items.size());
            for (const Item &item: items)
                handles.push_back(item.handle());
            detail::api().eventSetDrops(mHandle, handles.data(), (uint32_t) handles.size());
        }

        int32_t experience() const {
            return detail::api().eventExperience(mHandle);
        }

        void setExperience(int32_t experience) const {
            detail::api().eventSetExperience(mHandle, experience);
        }
    };
}
