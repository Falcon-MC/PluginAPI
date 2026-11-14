#pragma once

#include "falcon/Api.hpp"
#include "falcon/Entity.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace falcon {
    enum class DisplaySlot : uint32_t {
        Sidebar = FALCON_DISPLAY_SLOT_SIDEBAR,
        List = FALCON_DISPLAY_SLOT_LIST,
        BelowName = FALCON_DISPLAY_SLOT_BELOW_NAME
    };

    struct Score {
        /** The player's name, the entity's name tag or the fake player's name; empty for an unloaded entity. */
        std::string name;
        int32_t value = 0;
        /** The online player or loaded entity holding the score; not valid for a fake player. */
        Entity entity{nullptr};
    };

    /**
     * The server's scoreboard, shared with commands and scripts and saved with the default world. Objectives are
     * dummy objectives. A participant is an entity, a player through its entity, or a fake player named by text.
     */
    class Scoreboard {
    public:
        /** False when the name is taken. */
        static bool addObjective(const std::string &name, const std::string &displayName = "") {
            return detail::api().scoreboardAddObjective(name.c_str(), displayName.c_str()) != 0;
        }

        static bool removeObjective(const std::string &name) {
            return detail::api().scoreboardRemoveObjective(name.c_str()) != 0;
        }

        static std::vector<std::string> objectives() {
            std::vector<std::string> names;
            const uint32_t count = detail::api().scoreboardObjectiveCount();
            for (uint32_t index = 0; index < count; ++index)
                names.push_back(detail::text(detail::api().scoreboardObjectiveName(index)));
            return names;
        }

        static std::optional<std::string> displayName(const std::string &objective) {
            const char *name = detail::api().scoreboardObjectiveDisplayName(objective.c_str());
            if (name == nullptr)
                return std::nullopt;
            return std::string(name);
        }

        static std::optional<int32_t> score(const std::string &objective, const Entity &entity) {
            int32_t value = 0;
            if (detail::api().scoreboardEntityScore(objective.c_str(), entity.handle(), &value) == 0)
                return std::nullopt;
            return value;
        }

        static bool setScore(const std::string &objective, const Entity &entity, int32_t value) {
            return detail::api().scoreboardSetEntityScore(objective.c_str(), entity.handle(), value) != 0;
        }

        static bool resetScore(const std::string &objective, const Entity &entity) {
            return detail::api().scoreboardResetEntityScore(objective.c_str(), entity.handle()) != 0;
        }

        static std::optional<int32_t> score(const std::string &objective, const std::string &fakePlayer) {
            int32_t value = 0;
            if (detail::api().scoreboardFakePlayerScore(objective.c_str(), fakePlayer.c_str(), &value) == 0)
                return std::nullopt;
            return value;
        }

        static bool setScore(const std::string &objective, const std::string &fakePlayer, int32_t value) {
            return detail::api().scoreboardSetFakePlayerScore(objective.c_str(), fakePlayer.c_str(), value) != 0;
        }

        static bool resetScore(const std::string &objective, const std::string &fakePlayer) {
            return detail::api().scoreboardResetFakePlayerScore(objective.c_str(), fakePlayer.c_str()) != 0;
        }

        /** Every score of an objective, in the order the participants joined it. */
        static std::vector<Score> scores(const std::string &objective) {
            std::vector<Score> result;
            const uint32_t count = detail::api().scoreboardScoreCount(objective.c_str());
            for (uint32_t index = 0; index < count; ++index) {
                int32_t value = 0;
                const char *name = detail::api().scoreboardScoreAt(objective.c_str(), index, &value);
                if (name == nullptr)
                    continue;
                FalconEntity *holder = detail::api().scoreboardScoreEntity(objective.c_str(), index);
                result.push_back(Score{name, value, Entity(holder)});
            }
            return result;
        }

        /** Shows an objective in a slot, highest score first unless ascending. */
        static bool setDisplay(DisplaySlot slot, const std::string &objective, bool ascending = false) {
            return detail::api().scoreboardSetDisplay(static_cast<FalconDisplaySlot>(slot), objective.c_str(),
                                                      ascending ? 1 : 0) != 0;
        }

        static void clearDisplay(DisplaySlot slot) {
            detail::api().scoreboardSetDisplay(static_cast<FalconDisplaySlot>(slot), nullptr, 0);
        }

        static std::optional<std::string> displayed(DisplaySlot slot) {
            const char *name = detail::api().scoreboardDisplayObjective(static_cast<FalconDisplaySlot>(slot));
            if (name == nullptr)
                return std::nullopt;
            return std::string(name);
        }
    };
}
