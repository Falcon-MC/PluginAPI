#include "HelloPlugin/Tasks/AnnouncementTask.h"

#include <string>

void AnnouncementTask::start(falcon::Plugin &plugin) {
    falcon::Config &config = plugin.config();
    config.setDefault("announcement.message", std::string("This server runs Falcon"));
    config.setDefault("announcement.period-ticks", (int64_t) PERIOD_TICKS);
    plugin.saveConfig();

    const std::string message = config.getString("announcement.message");
    const int64_t period = config.getInt("announcement.period-ticks", (int64_t) PERIOD_TICKS);

    plugin.scheduler().repeat((uint64_t) (period > 0 ? period : (int64_t) PERIOD_TICKS), [message] {
        falcon::Server::broadcast(message);
    });
}
