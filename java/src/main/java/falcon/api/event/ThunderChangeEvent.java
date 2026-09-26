package falcon.api.event;

import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class ThunderChangeEvent extends Event {
    ThunderChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public boolean isThundering() {
        return eventState();
    }
}
