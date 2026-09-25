package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

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

    public void sendTitle(String title) {
        sendTitle(title, "");
    }

    public void sendTitle(String title, String subtitle) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSendTitle(mHandle, Interop.text(arena, title), Interop.text(arena, subtitle));
        }
    }

    public void sendActionBar(String message) {
        try (Arena arena = Arena.ofConfined()) {
            api().playerSendActionBar(mHandle, Interop.text(arena, message));
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
