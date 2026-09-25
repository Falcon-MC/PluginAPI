using Falcon;

namespace Hello.Content
{
    public static class ThunderWand
    {
        public const string ItemIdentifier = "hello:thunder_wand";

        public static void RegisterTo(Plugin plugin)
        {
            CustomItem wand = new CustomItem
            {
                Identifier = ItemIdentifier,
                DisplayName = "Thunder Wand",
                Icon = "thunder_wand",
                CreativeCategory = "equipment",
                MaxStackSize = 1,
                HandEquipped = true,
                OnUseOnBlock = StrikeLightning
            };

            if (!plugin.Content.AddItem(wand))
            {
                plugin.Logger.Warning("Could not register the thunder wand");
            }
        }

        private static bool StrikeLightning(Player player, Item item, BlockPos position, uint face)
        {
            player.Entity.Level.StrikeLightning(position.Center);
            return true;
        }
    }
}
