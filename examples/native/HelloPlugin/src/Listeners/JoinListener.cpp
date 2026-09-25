#include "HelloPlugin/Listeners/JoinListener.h"

#include "HelloPlugin/Storage/JoinStore.h"

#include <string>

void JoinListener::registerTo(const falcon::Plugin &plugin, JoinStore &joins) {
    plugin.events().on<falcon::PlayerJoinEvent>([&joins](falcon::PlayerJoinEvent &event) {
        const falcon::Player player = event.player();
        const uint32_t count = joins.recordJoin(player.name());

        if (count == 1)
            player.sendMessage("Welcome to the server, " + player.name() + "!");
        else
            player.sendMessage("Welcome back, " + player.name() + "! Visit number " + std::to_string(count) + ".");
    });

    plugin.events().on<falcon::PlayerQuitEvent>([&plugin](falcon::PlayerQuitEvent &event) {
        plugin.logger().info(event.player().name() + " left the server");
    });
}
