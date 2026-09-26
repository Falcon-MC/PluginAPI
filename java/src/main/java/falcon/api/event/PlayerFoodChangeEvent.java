package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerFoodChangeEvent extends Event {
    PlayerFoodChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public double food() {
        return eventAmount();
    }

    public void setFood(double food) {
        eventSetAmount(food);
    }

    public double previousFood() {
        return eventPreviousAmount();
    }
}
