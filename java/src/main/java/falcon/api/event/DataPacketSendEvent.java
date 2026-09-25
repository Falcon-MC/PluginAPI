package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class DataPacketSendEvent extends DataPacketEvent {
    DataPacketSendEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }
}
