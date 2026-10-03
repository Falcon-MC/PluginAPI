package falcon.api.event;

import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * Fired when a client opens a connection, before it sends any game packet. Cancelling it closes the
 * connection without a disconnect screen.
 */
public final class ConnectionOpenEvent extends Event {
    ConnectionOpenEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public String address() {
        return Interop.string(api().eventAddress(mHandle));
    }

    /**
     * The transport the client connected with: "raknet" or "nethernet".
     */
    public String transport() {
        return Interop.string(api().eventTransport(mHandle));
    }

    /**
     * The GUID the client announced during the RakNet handshake, or 0 on other transports.
     */
    public long clientGuid() {
        return api().eventClientGuid(mHandle);
    }

    /**
     * The datagram size negotiated during the RakNet handshake, or 0 on other transports.
     */
    public int mtuSize() {
        return api().eventMtuSize(mHandle);
    }
}
