#pragma once

#include "Command/Command.h"

#include <string>
#include <vector>

namespace garden {
    class GardenCommand : public Command {
    public:
        GardenCommand();

        bool execute(CommandOrigin &sender, const std::vector<std::string> &arguments) override;

        CommandPermission getRequiredPermission() const override;
    };
}
