package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

public final class EntityDamageEvent extends Event {
    EntityDamageEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    public Optional<Entity> attacker() {
        return optionalEntity(api().eventAttacker(mHandle));
    }

    public double amount() {
        return api().eventAmount(mHandle);
    }

    public void setAmount(double amount) {
        api().eventSetAmount(mHandle, amount);
    }

    public String cause() {
        return eventCause();
    }
}
