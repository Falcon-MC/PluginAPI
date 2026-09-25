#include "GardenPlugin/GardenPlugin.h"

#include "Core/Debug/BedrockLog.h"
#include "GardenPlugin/Commands/GardenCommand.h"
#include "GardenPlugin/Listeners/JoinListener.h"
#include "GardenPlugin/Storage/GardenStats.h"

#include <memory>

namespace garden {
    void GardenPlugin::onLoad() {
        LOG_INFO(LogAreaID::Server, "[%s] Saplings, sticks and chickens now follow the garden rules",
                 getName().c_str());
    }

    bool GardenPlugin::onEnable() {
        JoinListener::registerTo(*this);
        return registerCommand(std::make_shared<GardenCommand>());
    }

    void GardenPlugin::onDisable() {
        LOG_INFO(LogAreaID::Server, "[%s] %u tree(s) grown this session", getName().c_str(),
                 GardenStats::getTreesGrown());
    }
}

FALCON_INTERNAL_PLUGIN(garden::GardenPlugin)
