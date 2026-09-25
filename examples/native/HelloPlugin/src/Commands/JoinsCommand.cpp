#include "HelloPlugin/Commands/JoinsCommand.h"

#include "HelloPlugin/Storage/JoinStore.h"

#include <string>

void JoinsCommand::registerTo(const falcon::Plugin &plugin, JoinStore &joins) {
    plugin.commands().add("joins", "Shows how many times a player joined", "/joins [player]",
                          [&joins](falcon::CommandContext &context) {
                              const std::vector<std::string> &arguments = context.arguments();
                              const std::string name = arguments.empty() ? context.senderName() : arguments[0];
                              context.reply(name + " joined " + std::to_string(joins.joinsOf(name)) + " time(s)");
                              return true;
                          }, falcon::CommandPermission::Any);
}
