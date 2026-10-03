package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;

public final class PlayerPreLoginEvent extends Event {
    PlayerPreLoginEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public String kickMessage() {
        return eventMessage();
    }

    public void setKickMessage(String message) {
        eventSetMessage(message);
    }

    /**
     * The client data sent in the login request, as the JSON object the client signed.
     */
    public String clientData() {
        return Interop.string(api().eventClientData(mHandle));
    }

    /**
     * One top-level field of the client data, as text; empty when the client did not send it.
     */
    public String clientDataField(String key) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.string(api().eventClientDataField(mHandle, Interop.text(arena, key)));
        }
    }

    public String serverAddress() {
        return clientDataField("ServerAddress");
    }

    public String gameVersion() {
        return clientDataField("GameVersion");
    }

    public String deviceModel() {
        return clientDataField("DeviceModel");
    }

    public String deviceOS() {
        return clientDataField("DeviceOS");
    }

    public String thirdPartyName() {
        return clientDataField("ThirdPartyName");
    }
}
