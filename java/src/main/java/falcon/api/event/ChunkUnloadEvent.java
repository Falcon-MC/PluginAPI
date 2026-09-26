package falcon.api.event;

import falcon.api.Level;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class ChunkUnloadEvent extends Event {
    ChunkUnloadEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Level level() {
        return eventLevel();
    }

    public int chunkX() {
        return eventChunkX();
    }

    public int chunkZ() {
        return eventChunkZ();
    }
}
