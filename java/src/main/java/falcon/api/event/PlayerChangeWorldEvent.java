package falcon.api.event;

import falcon.api.Level;
import falcon.api.Player;
import falcon.api.Vec3;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerChangeWorldEvent extends Event {
    PlayerChangeWorldEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String world() {
        return Interop.string(api().eventWorldName(mHandle));
    }

    public String previousWorld() {
        return Interop.string(api().eventPreviousWorldName(mHandle));
    }

    public Level level() {
        return eventLevel();
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
