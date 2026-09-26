package falcon.api.event;

import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class WeatherChangeEvent extends Event {
    WeatherChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public boolean isRaining() {
        return eventState();
    }
}
