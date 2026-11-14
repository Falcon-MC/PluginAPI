package falcon.api;

import java.util.Optional;

/**
 * A score of an objective. The name is the player's name, the entity's name tag or the fake player's name, empty
 * for an unloaded entity; the entity is present while the player is online or the entity loaded.
 */
public record Score(String name, int value, Optional<Entity> entity) {
}
