package falcon.examples.hello.listeners;

import falcon.api.BlockPos;
import falcon.api.Plugin;
import falcon.api.event.BlockBreakEvent;
import falcon.api.event.EventPriority;

import java.util.Set;

public final class BlockBreakListener {
    private static final Set<String> DIAMOND_ORES = Set.of("minecraft:diamond_ore", "minecraft:deepslate_diamond_ore");

    private BlockBreakListener() {
    }

    public static void registerTo(Plugin plugin) {
        plugin.events().on(BlockBreakEvent.class, event -> {
            if (!DIAMOND_ORES.contains(event.blockName())) {
                return;
            }

            BlockPos position = event.position();
            event.player().sendActionBar("Diamonds found at " + position.x() + " " + position.y() + " "
                                         + position.z());
        }, EventPriority.MONITOR, true);
    }
}
