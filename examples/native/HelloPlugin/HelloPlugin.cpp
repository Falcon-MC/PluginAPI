#include <falcon/Falcon.hpp>

#include <string>

class HelloPlugin : public falcon::Plugin {
public:
    bool onEnable() override {
        events().on<falcon::PlayerJoinEvent>([](falcon::PlayerJoinEvent &event) {
            event.player().sendMessage("Welcome to the server, " + event.player().name() + "!");
        });

        events().on<falcon::PlayerQuitEvent>([this](falcon::PlayerQuitEvent &event) {
            logger().info(event.player().name() + " left the server");
        });

        events().on<falcon::PlayerChatEvent>([](falcon::PlayerChatEvent &event) {
            if (event.message() == "ping")
                event.player().sendMessage("pong");
        }, falcon::EventPriority::Monitor);

        commands().add("hello", "Says hello", "/hello", [](falcon::CommandContext &context) {
            context.reply("Hello, " + context.senderName() + "!");
            return true;
        }, falcon::CommandPermission::Any);

        scheduler().repeat(20 * 60 * 5, [] {
            falcon::Server::broadcast("This server runs Falcon");
        });

        logger().info("HelloPlugin enabled");
        return true;
    }

    void onDisable() override {
        logger().info("HelloPlugin disabled");
    }
};

FALCON_PLUGIN(HelloPlugin)
