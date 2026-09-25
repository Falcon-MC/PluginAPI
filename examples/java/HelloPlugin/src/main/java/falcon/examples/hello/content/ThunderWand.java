package falcon.examples.hello.content;

import falcon.api.Plugin;
import falcon.api.content.CustomItem;

public final class ThunderWand {
    public static final String IDENTIFIER = "hello:thunder_wand";

    private ThunderWand() {
    }

    public static void registerTo(Plugin plugin) {
        CustomItem wand = new CustomItem(IDENTIFIER)
                .displayName("Thunder Wand")
                .icon("thunder_wand")
                .creativeCategory("equipment")
                .maxStackSize(1)
                .handEquipped(true)
                .onUseOnBlock((player, item, position, face) -> {
                    player.entity().level().strikeLightning(position.center());
                    return true;
                });

        if (!plugin.content().item(wand)) {
            plugin.logger().warning("Could not register the thunder wand");
        }
    }
}
