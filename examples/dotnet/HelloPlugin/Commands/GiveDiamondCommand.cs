using System.Globalization;
using Falcon;

namespace Hello.Commands
{
    public static class GiveDiamondCommand
    {
        public const string Diamond = "minecraft:diamond";

        public const string PermissionNode = "hello.command.give-diamond";

        public static void RegisterTo(Plugin plugin)
        {
            plugin.Permissions.Add(PermissionNode, PermissionDefault.Operator);
            plugin.Commands.Add("give-diamond", "Gives you a shiny diamond", "/give-diamond [count]", Execute,
                                CommandPermission.Any);
        }

        private static bool Execute(CommandContext context)
        {
            if (!context.HasPermission(PermissionNode))
            {
                context.Reply("You are not allowed to use this command");
                return true;
            }

            Player? player = context.Player;
            if (player == null)
            {
                context.Reply("Only players can receive diamonds");
                return true;
            }

            uint count = 1;
            if (context.Arguments.Count > 0
                && !uint.TryParse(context.Arguments[0], NumberStyles.None, CultureInfo.InvariantCulture, out count))
            {
                return false;
            }

            using Item? diamond = Item.Create(Diamond, count);
            if (diamond == null)
            {
                context.Reply("Could not create the diamond");
                return true;
            }

            diamond.CustomName = "Shiny Diamond";
            diamond.Lore = new[] { "A gift from HelloPlugin" };
            if (!player.Inventory.Give(diamond))
            {
                Entity entity = player.Entity;
                entity.Level.DropItem(entity.Position, diamond);
            }

            context.Reply("Here you go!");
            return true;
        }
    }
}
