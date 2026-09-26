package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class FurnaceBurnEvent extends Event {
    FurnaceBurnEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public Item fuel() {
        return eventItem();
    }

    public int burnTime() {
        return (int) eventAmount();
    }

    public void setBurnTime(int ticks) {
        eventSetAmount(ticks);
    }
}
