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

    /** A dimension of a loaded world, or empty when no world has that name. */
    public Optional<Level> level(String world, Dimension dimension) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment level = api().worldLevel(Interop.text(arena, world), dimension.value());
            if (Interop.isNull(level)) {
                return Optional.empty();
            }
            return Optional.of(new Level(mContext, level));
        }
    }

    public List<String> worlds() {
        int count = api().serverWorldCount();
        List<String> names = new ArrayList<>(count);
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().serverWorldName(index)));
        }
        return names;
    }

    public String defaultWorld() {
        return Interop.string(api().serverDefaultWorldName());
    }

    public boolean loadWorld(String name) {
        return loadWorld(name, false, null);
    }

    /** Loads a world from disk, creating it when {@code create} is set. A null or empty seed picks a random one. */
    public boolean loadWorld(String name, boolean create, String seed) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment seedText = seed == null || seed.isEmpty() ? MemorySegment.NULL : Interop.text(arena, seed);
            return api().serverLoadWorld(Interop.text(arena, name), create ? 1 : 0, seedText) != 0;
        }
    }

    /** Saves and closes a world; fails for the default world and for worlds that still hold players. */
    public boolean unloadWorld(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverUnloadWorld(Interop.text(arena, name)) != 0;
        }
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
