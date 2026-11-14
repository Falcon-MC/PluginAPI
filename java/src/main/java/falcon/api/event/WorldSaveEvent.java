package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * A world is being saved; not cancellable.
 */
public final class WorldSaveEvent extends Event {
    WorldSaveEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public String world() {
        return eventWorldName();
    }
}
