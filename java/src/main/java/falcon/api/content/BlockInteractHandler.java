package falcon.api.content;

import falcon.api.BlockPos;
import falcon.api.Player;

@FunctionalInterface
public interface BlockInteractHandler {
    boolean interact(Player player, BlockPos position, int face);
}
