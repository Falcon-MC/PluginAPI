package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public final class Server {
    private final PluginContext mContext;

    Server(PluginContext context) {
        mContext = context;
    }

    public String version() {
        return Interop.string(api().serverVersion());
    }

    public List<Player> onlinePlayers() {
        int count = api().onlinePlayerCount();
        List<Player> players = new ArrayList<>(count);
        for (int index = 0; index < count; index++) {
            players.add(new Player(mContext, api().onlinePlayer(index)));
        }
        return players;
    }

    public Optional<Player> findPlayer(String name) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment player = api().findPlayer(Interop.text(arena, name));
            if (Interop.isNull(player)) {
                return Optional.empty();
            }
            return Optional.of(new Player(mContext, player));
        }
    }

    public void broadcast(String message) {
        try (Arena arena = Arena.ofConfined()) {
            api().broadcastMessage(Interop.text(arena, message));
        }
    }

    public Optional<Level> level(Dimension dimension) {
        MemorySegment level = api().serverLevel(dimension.value());
        if (Interop.isNull(level)) {
            return Optional.empty();
        }
        return Optional.of(new Level(mContext, level));
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
