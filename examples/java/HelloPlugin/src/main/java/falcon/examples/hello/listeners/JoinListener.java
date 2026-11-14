package falcon.examples.hello.listeners;

import falcon.api.DisplaySlot;
import falcon.api.EffectType;
import falcon.api.Entity;
import falcon.api.Player;
import falcon.api.Plugin;
import falcon.api.Scoreboard;
import falcon.api.event.PlayerJoinEvent;
import falcon.api.event.PlayerQuitEvent;
import falcon.examples.hello.storage.JoinStore;

public final class JoinListener {
    private JoinListener() {
    }

    public static void registerTo(Plugin plugin, JoinStore joins) {
        Scoreboard scoreboard = plugin.server().scoreboard();
        scoreboard.addObjective("joins", "Visits");
        scoreboard.setDisplay(DisplaySlot.LIST, "joins");

        plugin.events().on(PlayerJoinEvent.class, event -> {
            Player player = event.player();
            int count = joins.recordJoin(player.name());

            if (count == 1) {
                player.sendMessage("Welcome to the server, " + player.name() + "!");
            } else {
                player.sendMessage("Welcome back, " + player.name() + "! Visit number " + count + ".");
            }

            Entity body = player.entity();
            scoreboard.setScore("joins", body, count);
            body.addEffect(EffectType.SPEED, 200, 1, true);
            player.playSound("random.levelup", body.position());
            player.spawnParticle("minecraft:villager_happy", body.position());
        });

        plugin.events().on(PlayerQuitEvent.class, event -> {
            plugin.logger().info(event.player().name() + " left the server");
        });
    }
}
