package falcon.api.event;

import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class ServerCommandEvent extends Event {
    ServerCommandEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public String command() {
        return eventMessage();
    }

    public void setCommand(String command) {
        eventSetMessage(command);
    }
}
