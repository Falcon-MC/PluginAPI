package falcon.api.event;

import falcon.api.Player;
import falcon.api.Item;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.List;

public final class PlayerDeathEvent extends Event {
    PlayerDeathEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String message() {
        return eventMessage();
    }

    public void setMessage(String message) {
        eventSetMessage(message);
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

    public boolean keepInventory() {
        return api().eventKeepInventory(mHandle) != 0;
    }

    public void setKeepInventory(boolean keep) {
        api().eventSetKeepInventory(mHandle, keep ? 1 : 0);
    }

    public boolean keepExperience() {
        return api().eventKeepExperience(mHandle) != 0;
    }

    public void setKeepExperience(boolean keep) {
        api().eventSetKeepExperience(mHandle, keep ? 1 : 0);
    }
}
