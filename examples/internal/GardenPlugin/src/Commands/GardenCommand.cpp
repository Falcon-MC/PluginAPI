#include "GardenPlugin/Commands/GardenCommand.h"

#include "GardenPlugin/Storage/GardenStats.h"

namespace garden {
    GardenCommand::GardenCommand() : Command("garden", "Shows how many trees the garden grew", "/garden") {
    }

    bool GardenCommand::execute(CommandOrigin &sender, const std::vector<std::string> &arguments) {
        (void) arguments;
        sender.sendMessage("Trees grown since the server started: " + std::to_string(GardenStats::getTreesGrown()));
        return true;
    }

    CommandPermission GardenCommand::getRequiredPermission() const {
        return CommandPermission::Any;
    }
}
