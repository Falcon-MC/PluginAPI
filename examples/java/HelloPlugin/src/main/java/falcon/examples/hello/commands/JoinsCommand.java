package falcon.examples.hello.commands;

import falcon.api.Plugin;
import falcon.api.command.CommandPermission;
import falcon.examples.hello.storage.JoinStore;

import java.util.List;

public final class JoinsCommand {
    private JoinsCommand() {
    }

    public static void registerTo(Plugin plugin, JoinStore joins) {
        plugin.commands().add("joins", "Shows how many times a player joined", "/joins [player]", context -> {
            List<String> arguments = context.arguments();
            String name = arguments.isEmpty() ? context.senderName() : arguments.get(0);
            context.reply(name + " joined " + joins.joinsOf(name) + " time(s)");
            return true;
        }, CommandPermission.ANY);
    }
}
