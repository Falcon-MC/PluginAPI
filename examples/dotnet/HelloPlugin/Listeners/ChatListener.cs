using Falcon;

namespace Hello.Listeners
{
    public static class ChatListener
    {
        public static void RegisterTo(Plugin plugin)
        {
            plugin.Events.On<PlayerChatEvent>(chatEvent =>
            {
                if (chatEvent.Message == "ping")
                {
                    chatEvent.Player.SendMessage("pong");
                }
            }, EventPriority.Monitor);
        }
    }
}
