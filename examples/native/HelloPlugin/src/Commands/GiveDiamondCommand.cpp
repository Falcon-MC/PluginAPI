#include "HelloPlugin/Commands/GiveDiamondCommand.h"

#include <cstdint>
#include <exception>
#include <optional>
#include <string>

void GiveDiamondCommand::registerTo(const falcon::Plugin &plugin) {
    plugin.permissions().add(PERMISSION, falcon::PermissionDefault::Operator);

    plugin.commands().add("give-diamond", "Gives you a shiny diamond", "/give-diamond [count]",
                          [](falcon::CommandContext &context) {
                              if (!context.hasPermission(PERMISSION)) {
                                  context.reply("You are not allowed to use this command");
                                  return true;
                              }

                              const std::optional<falcon::Player> player = context.player();
                              if (!player) {
                                  context.reply("Only players can receive diamonds");
                                  return true;
                              }

                              uint32_t count = 1;
                              if (!context.arguments().empty()) {
                                  try {
                                      count = static_cast<uint32_t>(std::stoul(context.arguments()[0]));
                                  } catch (const std::exception &) {
                                      return false;
                                  }
                              }

                              std::optional<falcon::Item> diamond = falcon::Item::create(DIAMOND, count);
                              if (!diamond) {
                                  context.reply("Could not create the diamond");
                                  return true;
                              }

                              diamond->setCustomName("Shiny Diamond");
                              diamond->setLore({"A gift from HelloPlugin"});
                              if (!player->inventory().give(*diamond))
                                  player->entity().level().dropItem(player->entity().position(), *diamond);

                              context.reply("Here you go!");
                              return true;
                          }, falcon::CommandPermission::Any);
}
