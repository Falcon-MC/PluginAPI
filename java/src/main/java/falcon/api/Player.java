package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.util.Optional;

public final class Player {
    private final PluginContext mContext;
    private final MemorySegment mHandle;

    public Player(PluginContext context, MemorySegment handle) {
        mContext = context;
        mHandle = handle == null ? MemorySegment.NULL : handle;
    }

    public boolean valid() {
        return !Interop.isNull(mHandle);
    }

    public String name() {
        return Interop.string(api().playerName(mHandle));
    }

    public boolean isOperator() {
        return api().playerIsOperator(mHandle) != 0;
    }

    public void setOperator(boolean value) {
        api().playerSetOperator(mHandle, value ? 1 : 0);
    }

    public void sendMessage(String message) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSendMessage(mHandle, Interop.text(arena, message));
        }
    }

    public void kick(String reason) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerKick(mHandle, Interop.text(arena, reason));
        }
    }

    public Entity entity() {
        return new Entity(mContext, api().playerEntity(mHandle));
    }

    public GameMode gameMode() {
        return GameMode.of(api().playerGameMode(mHandle));
    }

    public void setGameMode(GameMode gameMode) {
        api().playerSetGameMode(mHandle, gameMode.value());
    }

    public String xuid() {
        return Interop.string(api().playerXuid(mHandle));
    }

    public String uuid() {
        return Interop.string(api().playerUuid(mHandle));
    }

    public String address() {
        return Interop.string(api().playerAddress(mHandle));
    }

    /**
     * The network protocol of the player's game version, which can be older than the server's when it accepts
     * several versions.
     */
    public int protocolVersion() {
        return api().playerProtocolVersion(mHandle);
    }

    /**
     * Shows a form built from its JSON definition and returns its id, which the matching
     * PlayerFormResponseEvent carries. Returns 0 when the form could not be sent.
     */
    public int sendForm(String formJson) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerSendForm(mHandle, Interop.text(arena, formJson));
        }
    }

    /**
     * Sends the player to another server. Returns false when the address is empty or the port invalid.
     */
    public boolean transfer(String address, int port) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerTransfer(mHandle, Interop.text(arena, address), port) != 0;
        }
    }

    public boolean transfer(String address) {
        return transfer(address, 19132);
    }

    public void sendTitle(String title) {
        sendTitle(title, "");
    }

    public void sendTitle(String title, String subtitle) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSendTitle(mHandle, Interop.text(arena, title), Interop.text(arena, subtitle));
        }
    }

    /**
     * Shows a boss bar and returns its id. The newest bar of a player is displayed, the others wait behind it, and a
     * raid bar replaces them all while it is up. Percent goes from 0 to 1. Colors: 0 pink, 1 blue, 2 red, 3 green,
     * 4 yellow, 5 purple, 6 rebecca purple, 7 white.
     */
    public int addBossBar(String title, float percent, int color) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerAddBossBar(mHandle, Interop.text(arena, title), percent, color);
        }
    }

    public boolean updateBossBar(int bar, String title, float percent, int color) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerUpdateBossBar(mHandle, bar, Interop.text(arena, title), percent, color) != 0;
        }
    }

    public boolean removeBossBar(int bar) {
        return api().playerRemoveBossBar(mHandle, bar) != 0;
    }

    /** Gives a filled image map of this player's world; dropped at the feet when the inventory is full. */
    public boolean giveMap(long mapId) {
        return api().playerGiveMap(mHandle, mapId) != 0;
    }

    public void sendActionBar(String message) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSendActionBar(mHandle, Interop.text(arena, message));
        }
    }

    /**
     * The spawn point a bed, an anchor or /spawnpoint set; empty when the player has none.
     */
    public Optional<SpawnPoint> spawnPoint() {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment position = arena.allocate(FalconAbi.BlockPos.LAYOUT);
            MemorySegment dimension = arena.allocate(ValueLayout.JAVA_INT);
            if (api().playerSpawnPoint(mHandle, position, dimension) == 0) {
                return Optional.empty();
            }
            return Optional.of(new SpawnPoint(Interop.blockPos(position),
                    Dimension.of(dimension.get(ValueLayout.JAVA_INT, 0))));
        }
    }

    public boolean setSpawnPoint(Level level, BlockPos position) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerSetSpawnPoint(mHandle, level.handle(), Interop.blockPos(arena, position)) != 0;
        }
    }

    public void clearSpawnPoint() {
        api().playerClearSpawnPoint(mHandle);
    }

    /**
     * The language code of the client, such as en_US.
     */
    public String language() {
        return Interop.string(api().playerLanguage(mHandle));
    }

    /**
     * The platform number the client sent at login, in the game's own numbering.
     */
    public int deviceOs() {
        return api().playerDeviceOs(mHandle);
    }

    public String skinId() {
        return Interop.string(api().playerSkinId(mHandle));
    }

    public SkinImage skinImage() {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment width = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment height = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment length = arena.allocate(ValueLayout.JAVA_INT);
            MemorySegment data = api().playerSkinImage(mHandle, width, height, length);
            int size = length.get(ValueLayout.JAVA_INT, 0);
            byte[] rgba = Interop.isNull(data) || size <= 0 ? new byte[0]
                                                           : data.reinterpret(size).toArray(ValueLayout.JAVA_BYTE);
            return new SkinImage(width.get(ValueLayout.JAVA_INT, 0), height.get(ValueLayout.JAVA_INT, 0), rgba);
        }
    }

    /**
     * Switches the camera to a preset such as minecraft:free, easing over easeTime seconds when above 0.
     */
    public boolean setCamera(String preset, float easeTime, CameraEase ease) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerSetCamera(mHandle, Interop.text(arena, preset), MemorySegment.NULL, MemorySegment.NULL,
                    easeTime, ease.value()) != 0;
        }
    }

    /**
     * A preset placed at a position and rotation (x pitch, y yaw, in degrees).
     */
    public boolean setCamera(String preset, Vec3 position, Vec3 rotation, float easeTime, CameraEase ease) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerSetCamera(mHandle, Interop.text(arena, preset), Interop.vec3(arena, position),
                    Interop.vec3(arena, rotation), easeTime, ease.value()) != 0;
        }
    }

    /**
     * Fades the screen to a 0xRRGGBB color and back, times in seconds.
     */
    public boolean fadeCamera(float fadeIn, float hold, float fadeOut, int color) {
        return api().playerFadeCamera(mHandle, fadeIn, hold, fadeOut, color) != 0;
    }

    public void clearCamera() {
        api().playerClearCamera(mHandle);
    }

    /**
     * Ticks left on the cooldown of the item's category, 0 when it is ready.
     */
    public int itemCooldown(Item item) {
        return api().playerItemCooldown(mHandle, item.handle());
    }

    public boolean startItemCooldown(Item item, int ticks) {
        return api().playerStartItemCooldown(mHandle, item.handle(), ticks) != 0;
    }

    /**
     * Runs a command line, without the slash, as this player, with the player's permissions.
     */
    public boolean executeCommand(String commandLine) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerExecuteCommand(mHandle, Interop.text(arena, commandLine)) != 0;
        }
    }

    public void playSound(String sound, Vec3 position) {
        playSound(sound, position, 1.0f, 1.0f);
    }

    /**
     * Plays a sound only this player hears, from a position in its level.
     */
    public void playSound(String sound, Vec3 position, float volume, float pitch) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerPlaySound(mHandle, Interop.text(arena, sound), Interop.vec3(arena, position), volume, pitch);
        }
    }

    public void stopAllSounds() {
        api().playerStopSound(mHandle, MemorySegment.NULL);
    }

    public void stopSound(String sound) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerStopSound(mHandle, Interop.nullableText(arena, sound));
        }
    }

    /**
     * Shows a particle effect only this player sees, in its level.
     */
    public void spawnParticle(String particle, Vec3 position) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSpawnParticle(mHandle, Interop.text(arena, particle), Interop.vec3(arena, position));
        }
    }

    public float food() {
        return api().playerFood(mHandle);
    }

    public void setFood(float food) {
        api().playerSetFood(mHandle, food);
    }

    public int xpLevel() {
        return api().playerXpLevel(mHandle);
    }

    public void setXpLevel(int level) {
        api().playerSetXpLevel(mHandle, level);
    }

    public PlayerInventory inventory() {
        return new PlayerInventory(mHandle);
    }

    public boolean hasPermission(String node) {
        try (Arena arena = Arena.ofConfined()) {
            return api().playerHasPermission(mHandle, Interop.text(arena, node)) != 0;
        }
    }

    public void setPermission(String node) {
        setPermission(node, true);
    }

    public void setPermission(String node, boolean value) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSetPermission(mContext.handle(), mHandle, Interop.text(arena, node), value ? 1 : 0);
        }
    }

    public void unsetPermission(String node) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerUnsetPermission(mContext.handle(), mHandle, Interop.text(arena, node));
        }
    }

    public boolean sendPacket(int packetId, byte[] data) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment bytes = MemorySegment.NULL;
            if (data.length > 0) {
                bytes = arena.allocateFrom(ValueLayout.JAVA_BYTE, data);
            }
            return api().playerSendPacket(mHandle, packetId, bytes, data.length) != 0;
        }
    }

    public MemorySegment handle() {
        return mHandle;
    }

    @Override
    public boolean equals(Object other) {
        return other instanceof Player player && player.mHandle.address() == mHandle.address();
    }

    @Override
    public int hashCode() {
        return Long.hashCode(mHandle.address());
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
