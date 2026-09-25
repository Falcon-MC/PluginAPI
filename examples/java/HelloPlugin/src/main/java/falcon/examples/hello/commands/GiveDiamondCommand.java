package falcon.examples.hello.commands;

import falcon.api.Entity;
import falcon.api.Item;
import falcon.api.PermissionDefault;
import falcon.api.Player;
import falcon.api.Plugin;
import falcon.api.command.CommandContext;
import falcon.api.command.CommandPermission;

import java.util.List;
import java.util.Optional;

public final class GiveDiamondCommand {
    public static final String DIAMOND = "minecraft:diamond";
    public static final String PERMISSION = "hello.command.give-diamond";

    private GiveDiamondCommand() {
    }

    public static void registerTo(Plugin plugin) {
        plugin.permissions().add(PERMISSION, PermissionDefault.OPERATOR);
        plugin.commands().add("give-diamond", "Gives you a shiny diamond", "/give-diamond [count]",
                GiveDiamondCommand::execute, CommandPermission.ANY);
    }

    private static boolean execute(CommandContext context) {
        if (!context.hasPermission(PERMISSION)) {
            context.reply("You are not allowed to use this command");
            return true;
        }

        Optional<Player> player = context.player();
        if (player.isEmpty()) {
            context.reply("Only players can receive diamonds");
            return true;
        }

        int count = 1;
        if (!context.arguments().isEmpty()) {
            try {
                count = Integer.parseUnsignedInt(context.arguments().get(0));
            } catch (NumberFormatException exception) {
                return false;
            }
        }

        Optional<Item> created = Item.create(DIAMOND, count);
        if (created.isEmpty()) {
            context.reply("Could not create the diamond");
            return true;
        }

        try (Item diamond = created.get()) {
            diamond.setCustomName("Shiny Diamond");
            diamond.setLore(List.of("A gift from HelloPlugin"));
            if (!player.get().inventory().give(diamond)) {
                Entity entity = player.get().entity();
                entity.level().dropItem(entity.position(), diamond);
            }
        }

        context.reply("Here you go!");
        return true;
    }
}
