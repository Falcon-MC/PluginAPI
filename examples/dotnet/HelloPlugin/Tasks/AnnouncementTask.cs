using Falcon;

namespace Hello.Tasks
{
    public static class AnnouncementTask
    {
        public const long PeriodTicks = 20 * 60 * 5;

        public static void Start(Plugin plugin)
        {
            Config config = plugin.Config;
            config.SetDefault("announcement.message", "This server runs Falcon");
            config.SetDefault("announcement.period-ticks", PeriodTicks);
            plugin.SaveConfig();

            string message = config.GetString("announcement.message");
            long period = config.GetInt("announcement.period-ticks", PeriodTicks);

            plugin.Scheduler.Repeat((ulong)(period > 0 ? period : PeriodTicks), () =>
            {
                Server.Broadcast(message);
            });
        }
    }
}
