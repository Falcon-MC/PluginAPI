package falcon.api.event;

import falcon.api.Dimension;
import falcon.api.Player;
import falcon.api.Vec3;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerChangeDimensionEvent extends Event {
    PlayerChangeDimensionEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public Dimension dimension() {
        return Dimension.of(api().eventDimension(mHandle));
    }

    public Dimension previousDimension() {
        return Dimension.of(api().eventPreviousDimension(mHandle));
    }

    public Vec3 from() {
        return eventFrom();
    }

    public Vec3 to() {
        return eventTo();
    }

    public void setTo(Vec3 position) {
        eventSetTo(position);
    }
}
