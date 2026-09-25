using Falcon;
using Hello.Storage;

namespace Hello.Listeners
{
    public static class JoinListener
    {
        public static void RegisterTo(Plugin plugin, JoinStore joins)
        {
            plugin.Events.On<PlayerJoinEvent>(joinEvent =>
            {
                Player player = joinEvent.Player;
                uint count = joins.RecordJoin(player.Name);

                if (count == 1)
                {
                    player.SendMessage("Welcome to the server, " + player.Name + "!");
                }
                else
                {
                    player.SendMessage("Welcome back, " + player.Name + "! Visit number " + count + ".");
                }
            });

            plugin.Events.On<PlayerQuitEvent>(quitEvent =>
            {
                plugin.Logger.Info(quitEvent.Player.Name + " left the server");
            });
        }
    }
}
