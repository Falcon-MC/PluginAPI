package falcon.api.content;

import falcon.api.BlockPos;
import falcon.api.Player;

@FunctionalInterface
public interface BlockBreakHandler {
    void broken(Player player, BlockPos position);
}
