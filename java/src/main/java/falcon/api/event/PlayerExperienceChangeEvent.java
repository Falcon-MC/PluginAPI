package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerExperienceChangeEvent extends Event {
    PlayerExperienceChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public int amount() {
        return (int) eventAmount();
    }

    public void setAmount(int amount) {
        eventSetAmount(amount);
    }
}
