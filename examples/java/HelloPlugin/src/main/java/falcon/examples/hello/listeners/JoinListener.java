package falcon.examples.hello.listeners;

import falcon.api.Player;
import falcon.api.Plugin;
import falcon.api.event.PlayerJoinEvent;
import falcon.api.event.PlayerQuitEvent;
import falcon.examples.hello.storage.JoinStore;

public final class JoinListener {
    private JoinListener() {
    }

    public static void registerTo(Plugin plugin, JoinStore joins) {
        plugin.events().on(PlayerJoinEvent.class, event -> {
            Player player = event.player();
            int count = joins.recordJoin(player.name());

            if (count == 1) {
                player.sendMessage("Welcome to the server, " + player.name() + "!");
            } else {
                player.sendMessage("Welcome back, " + player.name() + "! Visit number " + count + ".");
            }
        });

        plugin.events().on(PlayerQuitEvent.class, event -> {
            plugin.logger().info(event.player().name() + " left the server");
        });
    }
}
