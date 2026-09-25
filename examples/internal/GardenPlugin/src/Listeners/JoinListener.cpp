#include "GardenPlugin/Listeners/JoinListener.h"

#include "Actor/ServerPlayer.h"
#include "Core/Event/GameEvents.h"

namespace garden {
    void JoinListener::registerTo(falcon::internal::Plugin &plugin) {
        plugin.subscribe(plugin.getServer().getEventBus().after().mPlayerJoin, [](PlayerJoinAfterEvent &event) {
            event.mPlayer.sendMessage("Right-click a sapling with an empty hand to grow it, or use a stick near "
                                      "saplings. Chickens follow wheat seeds. Type /garden for the count.");
        });
    }
}
