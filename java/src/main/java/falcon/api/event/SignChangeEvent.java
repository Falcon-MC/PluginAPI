package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class SignChangeEvent extends Event {
    SignChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public BlockPos position() {
        return eventBlockPosition();
    }

    public boolean isFrontSide() {
        return eventState();
    }

    public String text() {
        return eventMessage();
    }

    public void setText(String text) {
        eventSetMessage(text);
    }
}
