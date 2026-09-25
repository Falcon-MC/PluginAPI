#include "HelloPlugin/HelloPlugin.h"

#include "HelloPlugin/Commands/JoinsCommand.h"
#include "HelloPlugin/Listeners/ChatListener.h"
#include "HelloPlugin/Listeners/JoinListener.h"
#include "HelloPlugin/Tasks/AnnouncementTask.h"

bool HelloPlugin::onEnable() {
    mJoins.load(dataFolder() + "/joins.txt");

    JoinListener::registerTo(*this, mJoins);
    ChatListener::registerTo(*this);
    JoinsCommand::registerTo(*this, mJoins);
    AnnouncementTask::start(*this);

    logger().info("HelloPlugin enabled");
    return true;
}

void HelloPlugin::onDisable() {
    mJoins.save();
    logger().info("HelloPlugin disabled");
}

FALCON_PLUGIN(HelloPlugin)
