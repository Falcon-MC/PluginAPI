package falcon.api.content;

import falcon.api.Entity;
import falcon.api.Player;

@FunctionalInterface
public interface EntityInteractHandler {
    boolean interact(Entity entity, Player player);
}
