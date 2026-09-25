#include "HelloPlugin/HelloPlugin.h"

#include "HelloPlugin/Commands/GiveDiamondCommand.h"
#include "HelloPlugin/Commands/JoinsCommand.h"
#include "HelloPlugin/Content/ThunderWand.h"
#include "HelloPlugin/Listeners/BlockBreakListener.h"
#include "HelloPlugin/Listeners/ChatListener.h"
#include "HelloPlugin/Listeners/JoinListener.h"
#include "HelloPlugin/Tasks/AnnouncementTask.h"

void HelloPlugin::onLoad() {
    ThunderWand::registerTo(*this);
}

bool HelloPlugin::onEnable() {
    mJoins.load(dataFolder() + "/joins.txt");

    JoinListener::registerTo(*this, mJoins);
    ChatListener::registerTo(*this);
    BlockBreakListener::registerTo(*this);
    JoinsCommand::registerTo(*this, mJoins);
    GiveDiamondCommand::registerTo(*this);
    AnnouncementTask::start(*this);

    logger().info("HelloPlugin enabled");
    return true;
}

void HelloPlugin::onDisable() {
    mJoins.save();
    logger().info("HelloPlugin disabled");
}

FALCON_PLUGIN(HelloPlugin)
