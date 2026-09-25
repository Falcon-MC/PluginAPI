package falcon.api.event;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.MemorySegment;
import java.util.HashMap;
import java.util.Map;
import java.util.function.BiFunction;

final class EventType<T extends Event> {
    private static final Map<Class<?>, EventType<?>> TYPES = new HashMap<>();

    static {
        register(PlayerJoinEvent.class, FalconAbi.EVENT_PLAYER_JOIN, PlayerJoinEvent::new);
        register(PlayerQuitEvent.class, FalconAbi.EVENT_PLAYER_QUIT, PlayerQuitEvent::new);
        register(PlayerChatEvent.class, FalconAbi.EVENT_PLAYER_CHAT, PlayerChatEvent::new);
        register(BlockBreakEvent.class, FalconAbi.EVENT_BLOCK_BREAK, BlockBreakEvent::new);
        register(BlockPlaceEvent.class, FalconAbi.EVENT_BLOCK_PLACE, BlockPlaceEvent::new);
        register(PlayerInteractBlockEvent.class, FalconAbi.EVENT_PLAYER_INTERACT_BLOCK, PlayerInteractBlockEvent::new);
        register(EntityDamageEvent.class, FalconAbi.EVENT_ENTITY_DAMAGE, EntityDamageEvent::new);
        register(EntityDeathEvent.class, FalconAbi.EVENT_ENTITY_DEATH, EntityDeathEvent::new);
        register(PlayerDeathEvent.class, FalconAbi.EVENT_PLAYER_DEATH, PlayerDeathEvent::new);
        register(PlayerRespawnEvent.class, FalconAbi.EVENT_PLAYER_RESPAWN, PlayerRespawnEvent::new);
        register(PlayerMoveEvent.class, FalconAbi.EVENT_PLAYER_MOVE, PlayerMoveEvent::new);
        register(PlayerDropItemEvent.class, FalconAbi.EVENT_PLAYER_DROP_ITEM, PlayerDropItemEvent::new);
        register(PlayerPickupItemEvent.class, FalconAbi.EVENT_PLAYER_PICKUP_ITEM, PlayerPickupItemEvent::new);
        register(DataPacketReceiveEvent.class, FalconAbi.EVENT_DATA_PACKET_RECEIVE, DataPacketReceiveEvent::new);
        register(DataPacketSendEvent.class, FalconAbi.EVENT_DATA_PACKET_SEND, DataPacketSendEvent::new);
        register(PlayerInteractEntityEvent.class, FalconAbi.EVENT_PLAYER_INTERACT_ENTITY,
                PlayerInteractEntityEvent::new);
        register(InventoryOpenEvent.class, FalconAbi.EVENT_INVENTORY_OPEN, InventoryOpenEvent::new);
        register(InventoryCloseEvent.class, FalconAbi.EVENT_INVENTORY_CLOSE, InventoryCloseEvent::new);
        register(InventoryTransactionEvent.class, FalconAbi.EVENT_INVENTORY_TRANSACTION,
                InventoryTransactionEvent::new);
        register(CraftItemEvent.class, FalconAbi.EVENT_CRAFT_ITEM, CraftItemEvent::new);
        register(PlayerCommandEvent.class, FalconAbi.EVENT_PLAYER_COMMAND, PlayerCommandEvent::new);
        register(ServerCommandEvent.class, FalconAbi.EVENT_SERVER_COMMAND, ServerCommandEvent::new);
        register(ServerTickEvent.class, FalconAbi.EVENT_SERVER_TICK, ServerTickEvent::new);
        register(PlayerGameModeChangeEvent.class, FalconAbi.EVENT_PLAYER_GAME_MODE_CHANGE,
                PlayerGameModeChangeEvent::new);
        register(PlayerChangeDimensionEvent.class, FalconAbi.EVENT_PLAYER_CHANGE_DIMENSION,
                PlayerChangeDimensionEvent::new);
        register(ProjectileHitEvent.class, FalconAbi.EVENT_PROJECTILE_HIT, ProjectileHitEvent::new);
        register(ExplosionEvent.class, FalconAbi.EVENT_EXPLOSION, ExplosionEvent::new);
        register(FireSpreadEvent.class, FalconAbi.EVENT_FIRE_SPREAD, FireSpreadEvent::new);
        register(BlockBurnEvent.class, FalconAbi.EVENT_BLOCK_BURN, BlockBurnEvent::new);
        register(CustomEvent.class, FalconAbi.EVENT_CUSTOM, CustomEvent::new);
    }

    private final int mId;
    private final BiFunction<PluginContext, MemorySegment, T> mFactory;

    private EventType(int id, BiFunction<PluginContext, MemorySegment, T> factory) {
        mId = id;
        mFactory = factory;
    }

    private static <T extends Event> void register(Class<T> type, int id,
                                                   BiFunction<PluginContext, MemorySegment, T> factory) {
        TYPES.put(type, new EventType<>(id, factory));
    }

    @SuppressWarnings("unchecked")
    static <T extends Event> EventType<T> of(Class<T> type) {
        EventType<?> eventType = TYPES.get(type);
        if (eventType == null) {
            throw new IllegalArgumentException(type.getName() + " is not an event the server fires");
        }
        return (EventType<T>) eventType;
    }

    int id() {
        return mId;
    }

    T create(PluginContext context, MemorySegment handle) {
        return mFactory.apply(context, handle);
    }
}
