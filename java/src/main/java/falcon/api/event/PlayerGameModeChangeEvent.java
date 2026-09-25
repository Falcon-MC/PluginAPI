package falcon.api.event;

import falcon.api.GameMode;
import falcon.api.Player;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;

public final class PlayerGameModeChangeEvent extends Event {
    PlayerGameModeChangeEvent(PluginContext context, MemorySegment handle) {
        super(context, handle);
    }

    public Player player() {
        return eventPlayer();
    }

    public GameMode gameMode() {
        return GameMode.of(api().eventGameMode(mHandle));
    }

    public GameMode previousGameMode() {
        return GameMode.of(api().eventPreviousGameMode(mHandle));
    }
}
