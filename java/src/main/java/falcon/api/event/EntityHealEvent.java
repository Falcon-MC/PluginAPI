package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * An entity or player regains health; cancellable, the amount can change.
 */
public final class EntityHealEvent extends Event {
    EntityHealEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public double amount() {
        return eventAmount();
    }

    public void setAmount(double value) {
        eventSetAmount(value);
    }

    public String cause() {
        return eventCause();
    }
}
