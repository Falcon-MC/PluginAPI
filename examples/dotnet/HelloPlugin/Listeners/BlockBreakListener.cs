using Falcon;

namespace Hello.Listeners
{
    public static class BlockBreakListener
    {
        public static void RegisterTo(Plugin plugin)
        {
            plugin.Events.On<BlockBreakEvent>(breakEvent =>
            {
                string name = breakEvent.BlockName;
                if (name != "minecraft:diamond_ore" && name != "minecraft:deepslate_diamond_ore")
                {
                    return;
                }

                BlockPos position = breakEvent.Position;
                breakEvent.Player.SendActionBar("Diamonds found at " + position.X + " " + position.Y + " "
                                                + position.Z);
            }, EventPriority.Monitor, true);
        }
    }
}
