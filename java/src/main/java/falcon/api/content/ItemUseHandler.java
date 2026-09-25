package falcon.api.content;

import falcon.api.Item;
import falcon.api.Player;

@FunctionalInterface
public interface ItemUseHandler {
    boolean use(Player player, Item item);
}
