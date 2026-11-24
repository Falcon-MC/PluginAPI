package falcon.api.event;

import falcon.api.Entity;
import falcon.api.Item;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.List;

public final class EntityDeathEvent extends Event {
    EntityDeathEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Entity entity() {
        return eventEntity();
    }

    /** The stacks are borrowed until the event ends or the drops are replaced. */
    public List<Item> drops() {
        return eventDrops();
    }

    public void setDrops(List<Item> drops) {
        eventSetDrops(drops);
    }

    public int experience() {
        return api().eventExperience(mHandle);
    }

    public void setExperience(int experience) {
        api().eventSetExperience(mHandle, experience);
    }
}
