package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class RedstoneChangeEvent extends Event {
    RedstoneChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public int power() {
        return (int) eventAmount();
    }

    public void setPower(int power) {
        eventSetAmount(power);
    }

    public int previousPower() {
        return (int) eventPreviousAmount();
    }
}
