package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Entity;
import falcon.api.Level;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.Optional;

public final class ProjectileHitEvent extends Event {
    ProjectileHitEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity projectile() {
        return eventEntity();
    }

    public Optional<Entity> shooter() {
        return optionalEntity(api().eventAttacker(mHandle));
    }

    public Optional<Entity> target() {
        return optionalEntity(api().eventTarget(mHandle));
    }

    public boolean hitBlock() {
        return !eventBlockName().isEmpty();
    }

    public BlockPos blockPosition() {
        return eventBlockPosition();
    }

    public String blockName() {
        return eventBlockName();
    }

    public Vec3 position() {
        return eventPosition();
    }

    public Level level() {
        return eventLevel();
    }
}
