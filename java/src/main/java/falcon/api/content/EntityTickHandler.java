package falcon.api.content;

import falcon.api.Entity;

@FunctionalInterface
public interface EntityTickHandler {
    void tick(Entity entity);
}
