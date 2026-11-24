package falcon.api.event;

import falcon.api.Entity;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
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

    public record Knockback(float horizontal, float vertical) {
    }

    public Optional<Knockback> knockback() {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment horizontal = arena.allocate(ValueLayout.JAVA_FLOAT);
            MemorySegment vertical = arena.allocate(ValueLayout.JAVA_FLOAT);
            if (api().eventKnockback(mHandle, horizontal, vertical) == 0) {
                return Optional.empty();
            }
            return Optional.of(new Knockback(horizontal.get(ValueLayout.JAVA_FLOAT, 0),
                    vertical.get(ValueLayout.JAVA_FLOAT, 0)));
        }
    }

    public void setKnockback(float horizontal, float vertical) {
        api().eventSetKnockback(mHandle, horizontal, vertical);
    }

    public String cause() {
        return eventCause();
    }
}
