package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
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

    /**
     * The entity or player with that runtime id, empty once it is gone.
     */
    public Optional<Entity> entity(long runtimeId) {
        MemorySegment entity = api().serverEntity(runtimeId);
        return Interop.isNull(entity) ? Optional.empty() : Optional.of(new Entity(mContext, entity));
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

    /**
     * Brings in a player played by the server, as the game's simulated players: it joins like anyone, shows in the
     * player list and stays until removed. Empty when the name or the place is refused.
     */
    public Optional<Player> spawnSimulatedPlayer(String name, Level level, Vec3 position, float yaw, GameMode gameMode) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment player = api().serverSpawnSimulatedPlayer(Interop.text(arena, name), level.handle(),
                    Interop.vec3(arena, position), yaw, gameMode.value());
            if (Interop.isNull(player)) {
                return Optional.empty();
            }
            return Optional.of(new Player(mContext, player));
        }
    }

    public int blockNetworkId(String name, String statesJson) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverBlockNetworkId(Interop.text(arena, name), Interop.nullableText(arena, statesJson));
        }
    }

    public Optional<ItemInfo> itemInfo(String identifier) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment out = arena.allocate(FalconAbi.ItemInfo.LAYOUT);
            if (api().serverItemInfo(Interop.text(arena, identifier), out) == 0) {
                return Optional.empty();
            }
            return Optional.of(new ItemInfo(Interop.string(out.get(ValueLayout.ADDRESS, FalconAbi.ItemInfo.NAME)),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.MAX_STACK_SIZE),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.MAX_DURABILITY),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.TOOL_TYPE),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.TOOL_TIER),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.ATTACK_DAMAGE),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.ARMOR_SLOT),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.ARMOR_POINTS),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.ARMOR_TOUGHNESS),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.ItemInfo.NUTRITION),
                    out.get(ValueLayout.JAVA_FLOAT, FalconAbi.ItemInfo.SATURATION)));
        }
    }

    public Optional<BlockInfo> blockInfo(String identifier) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment out = arena.allocate(FalconAbi.BlockInfo.LAYOUT);
            if (api().serverBlockInfo(Interop.text(arena, identifier), out) == 0) {
                return Optional.empty();
            }
            return Optional.of(new BlockInfo(Interop.string(out.get(ValueLayout.ADDRESS, FalconAbi.BlockInfo.NAME)),
                    out.get(ValueLayout.JAVA_FLOAT, FalconAbi.BlockInfo.HARDNESS),
                    out.get(ValueLayout.JAVA_FLOAT, FalconAbi.BlockInfo.BLAST_RESISTANCE),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.TOOL_TYPE),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.TOOL_TIER),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.LIGHT_EMISSION),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.TRANSPARENT) != 0,
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.SOLID) != 0,
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.FLAME_ENCOURAGEMENT),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.FLAMMABILITY),
                    out.get(ValueLayout.JAVA_INT, FalconAbi.BlockInfo.MAX_STACK_SIZE)));
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

    /**
     * Bans a name, like /ban, and kicks the player when online; the reason is shown to them.
     */
    public boolean ban(String name, String reason) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverBan(Interop.text(arena, name), Interop.text(arena, reason),
                    api().pluginName(mContext.handle())) != 0;
        }
    }

    public boolean pardon(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverPardon(Interop.text(arena, name)) != 0;
        }
    }

    public boolean isBanned(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverIsBanned(Interop.text(arena, name)) != 0;
        }
    }

    public List<String> bannedNames() {
        int count = api().serverBanCount();
        List<String> names = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().serverBannedName(index)));
        }
        return names;
    }

    /**
     * Bans an address, like /ban-ip, and kicks every player connected from it.
     */
    public boolean banIp(String address, String reason) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverBanIp(Interop.text(arena, address), Interop.text(arena, reason),
                    api().pluginName(mContext.handle())) != 0;
        }
    }

    public boolean pardonIp(String address) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverPardonIp(Interop.text(arena, address)) != 0;
        }
    }

    public boolean isIpBanned(String address) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverIsIpBanned(Interop.text(arena, address)) != 0;
        }
    }

    public List<String> bannedIps() {
        int count = api().serverIpBanCount();
        List<String> addresses = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            addresses.add(Interop.string(api().serverBannedIp(index)));
        }
        return addresses;
    }

    public boolean allowListAdd(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverAllowListAdd(Interop.text(arena, name)) != 0;
        }
    }

    /**
     * Removes a name and kicks the players the allow list no longer lets in.
     */
    public boolean allowListRemove(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverAllowListRemove(Interop.text(arena, name)) != 0;
        }
    }

    public List<String> allowList() {
        int count = api().serverAllowListCount();
        List<String> names = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().serverAllowListName(index)));
        }
        return names;
    }

    public boolean allowListEnabled() {
        return api().serverAllowListEnabled() != 0;
    }

    public void setAllowListEnabled(boolean enabled) {
        api().serverSetAllowListEnabled(enabled ? 1 : 0);
    }

    public String motd() {
        return Interop.string(api().serverMotd());
    }

    /**
     * The server name of the server list entry, until the server restarts; server.properties is not changed.
     */
    public void setMotd(String motd) {
        try (Arena arena = Arena.ofConfined()) {
            api().serverSetMotd(Interop.text(arena, motd));
        }
    }

    public int maxPlayers() {
        return api().serverMaxPlayers();
    }

    public boolean setMaxPlayers(int maxPlayers) {
        return api().serverSetMaxPlayers(maxPlayers) != 0;
    }

    /**
     * Ticks per second over the last ticks, at most 20.
     */
    public double tps() {
        return api().serverTps();
    }

    public double millisecondsPerTick() {
        return api().serverMillisecondsPerTick();
    }

    /**
     * Stops the server after the current tick, like /stop.
     */
    public void shutdown() {
        api().serverShutdown();
    }

    public List<String> plugins() {
        int count = api().serverPluginCount();
        List<String> names = new ArrayList<>(Math.max(count, 0));
        for (int index = 0; index < count; index++) {
            names.add(Interop.string(api().serverPluginName(index)));
        }
        return names;
    }

    public Optional<String> pluginVersion(String name) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment version = api().serverPluginVersion(Interop.text(arena, name));
            return Interop.isNull(version) ? Optional.empty() : Optional.of(Interop.string(version));
        }
    }

    public boolean isPluginEnabled(String name) {
        try (Arena arena = Arena.ofConfined()) {
            return api().serverPluginEnabled(Interop.text(arena, name)) != 0;
        }
    }

    public Difficulty difficulty() {
        return Difficulty.of(api().serverDifficulty());
    }

    /**
     * Saved in the server properties and sent to every player, like /difficulty.
     */
    public boolean setDifficulty(Difficulty difficulty) {
        return api().serverSetDifficulty(difficulty.value()) != 0;
    }

    public Scoreboard scoreboard() {
        return new Scoreboard(mContext);
    }

    public Recipes recipes() {
        return new Recipes(mContext);
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
