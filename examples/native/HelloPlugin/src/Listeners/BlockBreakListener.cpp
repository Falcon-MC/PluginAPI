#include "HelloPlugin/Listeners/BlockBreakListener.h"

#include <string>

void BlockBreakListener::registerTo(const falcon::Plugin &plugin) {
    plugin.events().on<falcon::BlockBreakEvent>([](falcon::BlockBreakEvent &event) {
        const std::string name = event.blockName();
        if (name != "minecraft:diamond_ore" && name != "minecraft:deepslate_diamond_ore")
            return;

        const falcon::BlockPos position = event.position();
        event.player().sendActionBar("Diamonds found at " + std::to_string(position.x) + " " +
                                     std::to_string(position.y) + " " + std::to_string(position.z));
    }, falcon::EventPriority::Monitor, true);
}
