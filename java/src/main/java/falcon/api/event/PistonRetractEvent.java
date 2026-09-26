package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PistonRetractEvent extends PistonEvent {
    PistonRetractEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }
}
