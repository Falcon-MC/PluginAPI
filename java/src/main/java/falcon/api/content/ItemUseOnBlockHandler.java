package falcon.api.content;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Player;

@FunctionalInterface
public interface ItemUseOnBlockHandler {
    boolean use(Player player, Item item, BlockPos position, int face);
}
