package falcon.examples.hello.tasks;

import falcon.api.Config;
import falcon.api.Plugin;

public final class AnnouncementTask {
    public static final long PERIOD_TICKS = 20 * 60 * 5;

    private AnnouncementTask() {
    }

    public static void start(Plugin plugin) {
        Config config = plugin.config();
        config.setDefault("announcement.message", "This server runs Falcon");
        config.setDefault("announcement.period-ticks", PERIOD_TICKS);
        plugin.saveConfig();

        String message = config.getString("announcement.message");
        long period = config.getInt("announcement.period-ticks", PERIOD_TICKS);

        plugin.scheduler().repeat(period > 0 ? period : PERIOD_TICKS, () -> {
            plugin.server().broadcast(message);
        });
    }
}
