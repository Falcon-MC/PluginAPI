package falcon.api.event;

import falcon.api.BlockPos;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerBedLeaveEvent extends Event {
    PlayerBedLeaveEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public BlockPos bed() {
        return eventBlockPosition();
    }
}
