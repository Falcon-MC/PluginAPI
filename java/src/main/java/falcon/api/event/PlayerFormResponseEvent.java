package falcon.api.event;

import falcon.api.Player;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

/**
 * Fired when a player answers or closes a form sent with Player.sendForm.
 */
public final class PlayerFormResponseEvent extends Event {
    PlayerFormResponseEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public int formId() {
        return api().eventFormId(mHandle);
    }

    /**
     * The answer as JSON: the button index, a boolean or the array of custom form values. Empty when the
     * player closed the form.
     */
    public String response() {
        return Interop.string(api().eventFormResponse(mHandle));
    }

    public boolean closed() {
        return api().eventState(mHandle) != 0;
    }
}
