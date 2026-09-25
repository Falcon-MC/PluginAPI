#pragma once

#include "falcon/Entity.hpp"
#include "falcon/Player.hpp"

#include <functional>
#include <string>

namespace falcon {
    struct CustomEntity {
        std::string identifier;
        float width = 0.6f;
        float height = 1.8f;
        float maxHealth = 20.0f;
        bool summonable = true;
        std::function<void(Entity &)> onTick;
        std::function<bool(Entity &, Player &)> onInteract;
    };
}
