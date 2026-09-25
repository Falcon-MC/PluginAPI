#include "HelloPlugin/Tasks/AnnouncementTask.h"

void AnnouncementTask::start(const falcon::Plugin &plugin) {
    plugin.scheduler().repeat(PERIOD_TICKS, [] {
        falcon::Server::broadcast("This server runs Falcon");
    });
}
