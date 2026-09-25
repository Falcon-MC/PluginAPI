#pragma once

#include "falcon/Player.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace falcon {
    enum class CommandPermission : uint32_t {
        Any = FALCON_PERMISSION_ANY,
        Operator = FALCON_PERMISSION_OPERATOR
    };

    class CommandContext {
    public:
        CommandContext(FalconCommandSender *sender, std::vector<std::string> arguments)
                : mSender(sender), mArguments(std::move(arguments)) {
        }

        std::string senderName() const {
            return detail::api().senderName(mSender);
        }

        std::optional<Player> player() const {
            FalconPlayer *handle = detail::api().senderPlayer(mSender);
            if (handle == nullptr)
                return std::nullopt;
            return Player(handle);
        }

        const std::vector<std::string> &arguments() const {
            return mArguments;
        }

        void reply(const std::string &message) const {
            detail::api().senderSendMessage(mSender, message.c_str());
        }

    private:
        FalconCommandSender *mSender;
        std::vector<std::string> mArguments;
    };

    using CommandHandler = std::function<bool(CommandContext &)>;

    class Commands {
    public:
        bool add(const std::string &name, const std::string &description, const std::string &usage,
                 CommandHandler handler, CommandPermission permission = CommandPermission::Operator) const {
            auto *stored = detail::keep(std::make_unique<CommandHandler>(std::move(handler)));

            FalconCommandDescriptor descriptor;
            descriptor.name = name.c_str();
            descriptor.description = description.c_str();
            descriptor.usage = usage.c_str();
            descriptor.permission = (FalconCommandPermission) permission;
            descriptor.handler = &dispatch;
            descriptor.userData = stored;
            return detail::api().registerCommand(detail::plugin(), &descriptor) != 0;
        }

    private:
        static int dispatch(FalconCommandSender *sender, const char *const *arguments, uint32_t argumentCount,
                            void *userData) {
            int result = 0;
            detail::guarded("A command handler threw an exception", [&] {
                std::vector<std::string> values;
                values.reserve(argumentCount);
                for (uint32_t index = 0; index < argumentCount; index++)
                    values.emplace_back(arguments[index]);

                CommandContext context(sender, std::move(values));
                result = (*static_cast<CommandHandler *>(userData))(context) ? 1 : 0;
            });
            return result;
        }
    };
}
