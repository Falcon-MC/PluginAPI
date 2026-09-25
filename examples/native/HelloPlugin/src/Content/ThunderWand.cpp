#include "HelloPlugin/Content/ThunderWand.h"

#include <cstdint>
#include <utility>

void ThunderWand::registerTo(const falcon::Plugin &plugin) {
    falcon::CustomItem wand;
    wand.identifier = IDENTIFIER;
    wand.displayName = "Thunder Wand";
    wand.icon = "thunder_wand";
    wand.creativeCategory = "equipment";
    wand.maxStackSize = 1;
    wand.handEquipped = true;
    wand.onUseOnBlock = [](falcon::Player &player, falcon::Item &, const falcon::BlockPos &position, uint32_t) {
        player.entity().level().strikeLightning(position.center());
        return true;
    };

    if (!plugin.content().item(std::move(wand)))
        plugin.logger().warning("Could not register the thunder wand");
}
