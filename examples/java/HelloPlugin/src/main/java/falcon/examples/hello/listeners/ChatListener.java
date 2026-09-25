package falcon.examples.hello.listeners;

import falcon.api.Plugin;
import falcon.api.event.EventPriority;
import falcon.api.event.PlayerChatEvent;

public final class ChatListener {
    private ChatListener() {
    }

    public static void registerTo(Plugin plugin) {
        plugin.events().on(PlayerChatEvent.class, event -> {
            if (event.message().equals("ping")) {
                event.player().sendMessage("pong");
            }
        }, EventPriority.MONITOR);
    }
}
