package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Item;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class FurnaceSmeltEvent extends Event {
    FurnaceSmeltEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public Item source() {
        return eventItem();
    }

    public Item result() {
        return eventResult();
    }

    public void setResult(Item result) {
        eventSetResult(result);
    }
}
