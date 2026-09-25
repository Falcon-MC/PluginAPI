using Falcon;
using Hello.Storage;

namespace Hello.Commands
{
    public static class JoinsCommand
    {
        public static void RegisterTo(Plugin plugin, JoinStore joins)
        {
            plugin.Commands.Add("joins", "Shows how many times a player joined", "/joins [player]", context =>
            {
                string name = context.Arguments.Count == 0 ? context.SenderName : context.Arguments[0];
                context.Reply(name + " joined " + joins.JoinsOf(name) + " time(s)");
                return true;
            }, CommandPermission.Any);
        }
    }
}
