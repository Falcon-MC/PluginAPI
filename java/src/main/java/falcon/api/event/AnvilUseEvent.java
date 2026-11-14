package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A player took a result out of an anvil; not cancellable. The amount is the levels it cost.
 */
public final class AnvilUseEvent extends Event {
    AnvilUseEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public BlockPos anvil() {
        return eventBlockPosition();
    }

    public double levels() {
        return eventAmount();
    }

    public void setLevels(double value) {
        eventSetAmount(value);
    }
}
