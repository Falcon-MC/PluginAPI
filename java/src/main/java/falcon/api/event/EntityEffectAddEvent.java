package falcon.api.event;

import falcon.api.Effect;
import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * An entity or player gets a potion effect; cancellable.
 */
public final class EntityEffectAddEvent extends Event {
    EntityEffectAddEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Effect effect() {
        return eventEffect();
    }
}
