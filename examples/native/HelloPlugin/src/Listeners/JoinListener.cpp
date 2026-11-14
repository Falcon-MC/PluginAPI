#include "HelloPlugin/Listeners/JoinListener.h"

#include "HelloPlugin/Storage/JoinStore.h"

#include <string>

void JoinListener::registerTo(const falcon::Plugin &plugin, JoinStore &joins) {
    falcon::Scoreboard::addObjective("joins", "Visits");
    falcon::Scoreboard::setDisplay(falcon::DisplaySlot::List, "joins");

    plugin.events().on<falcon::PlayerJoinEvent>([&joins](falcon::PlayerJoinEvent &event) {
        const falcon::Player player = event.player();
        const uint32_t count = joins.recordJoin(player.name());

        if (count == 1)
            player.sendMessage("Welcome to the server, " + player.name() + "!");
        else
            player.sendMessage("Welcome back, " + player.name() + "! Visit number " + std::to_string(count) + ".");

        const falcon::Entity body = player.entity();
        falcon::Scoreboard::setScore("joins", body, (int32_t) count);
        body.addEffect(falcon::EffectType::Speed, 200, 1);
        player.playSound("random.levelup", body.position());
        player.spawnParticle("minecraft:villager_happy", body.position());
    });

    plugin.events().on<falcon::PlayerQuitEvent>([&plugin](falcon::PlayerQuitEvent &event) {
        plugin.logger().info(event.player().name() + " left the server");
    });
}
