#include "HelloPlugin/Listeners/ChatListener.h"

void ChatListener::registerTo(const falcon::Plugin &plugin) {
    plugin.events().on<falcon::PlayerChatEvent>([](falcon::PlayerChatEvent &event) {
        if (event.message() == "ping")
            event.player().sendMessage("pong");
    }, falcon::EventPriority::Monitor);
}
