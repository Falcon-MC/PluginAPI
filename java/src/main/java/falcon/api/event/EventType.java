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
        register(PlayerPreLoginEvent.class, FalconAbi.EVENT_PLAYER_PRE_LOGIN, PlayerPreLoginEvent::new);
        register(PlayerToggleSneakEvent.class, FalconAbi.EVENT_PLAYER_TOGGLE_SNEAK, PlayerToggleSneakEvent::new);
        register(PlayerToggleSprintEvent.class, FalconAbi.EVENT_PLAYER_TOGGLE_SPRINT, PlayerToggleSprintEvent::new);
        register(PlayerToggleFlightEvent.class, FalconAbi.EVENT_PLAYER_TOGGLE_FLIGHT, PlayerToggleFlightEvent::new);
        register(PlayerItemHeldEvent.class, FalconAbi.EVENT_PLAYER_ITEM_HELD, PlayerItemHeldEvent::new);
        register(PlayerItemConsumeEvent.class, FalconAbi.EVENT_PLAYER_ITEM_CONSUME, PlayerItemConsumeEvent::new);
        register(PlayerBedEnterEvent.class, FalconAbi.EVENT_PLAYER_BED_ENTER, PlayerBedEnterEvent::new);
        register(PlayerBedLeaveEvent.class, FalconAbi.EVENT_PLAYER_BED_LEAVE, PlayerBedLeaveEvent::new);
        register(PlayerJumpEvent.class, FalconAbi.EVENT_PLAYER_JUMP, PlayerJumpEvent::new);
        register(PlayerFoodChangeEvent.class, FalconAbi.EVENT_PLAYER_FOOD_CHANGE, PlayerFoodChangeEvent::new);
        register(PlayerExperienceChangeEvent.class, FalconAbi.EVENT_PLAYER_EXPERIENCE_CHANGE,
                PlayerExperienceChangeEvent::new);
        register(EntitySpawnEvent.class, FalconAbi.EVENT_ENTITY_SPAWN, EntitySpawnEvent::new);
        register(EntityTransformEvent.class, FalconAbi.EVENT_ENTITY_TRANSFORM, EntityTransformEvent::new);
        register(EntityTargetEvent.class, FalconAbi.EVENT_ENTITY_TARGET, EntityTargetEvent::new);
        register(ProjectileLaunchEvent.class, FalconAbi.EVENT_PROJECTILE_LAUNCH, ProjectileLaunchEvent::new);
        register(BlockGrowEvent.class, FalconAbi.EVENT_BLOCK_GROW, BlockGrowEvent::new);
        register(BlockSpreadEvent.class, FalconAbi.EVENT_BLOCK_SPREAD, BlockSpreadEvent::new);
        register(BlockFormEvent.class, FalconAbi.EVENT_BLOCK_FORM, BlockFormEvent::new);
        register(BlockFadeEvent.class, FalconAbi.EVENT_BLOCK_FADE, BlockFadeEvent::new);
        register(LiquidFlowEvent.class, FalconAbi.EVENT_LIQUID_FLOW, LiquidFlowEvent::new);
        register(PistonExtendEvent.class, FalconAbi.EVENT_PISTON_EXTEND, PistonExtendEvent::new);
        register(PistonRetractEvent.class, FalconAbi.EVENT_PISTON_RETRACT, PistonRetractEvent::new);
        register(RedstoneChangeEvent.class, FalconAbi.EVENT_REDSTONE_CHANGE, RedstoneChangeEvent::new);
        register(SignChangeEvent.class, FalconAbi.EVENT_SIGN_CHANGE, SignChangeEvent::new);
        register(LeavesDecayEvent.class, FalconAbi.EVENT_LEAVES_DECAY, LeavesDecayEvent::new);
        register(WeatherChangeEvent.class, FalconAbi.EVENT_WEATHER_CHANGE, WeatherChangeEvent::new);
        register(ThunderChangeEvent.class, FalconAbi.EVENT_THUNDER_CHANGE, ThunderChangeEvent::new);
        register(ChunkLoadEvent.class, FalconAbi.EVENT_CHUNK_LOAD, ChunkLoadEvent::new);
        register(ChunkUnloadEvent.class, FalconAbi.EVENT_CHUNK_UNLOAD, ChunkUnloadEvent::new);
        register(FurnaceSmeltEvent.class, FalconAbi.EVENT_FURNACE_SMELT, FurnaceSmeltEvent::new);
        register(FurnaceBurnEvent.class, FalconAbi.EVENT_FURNACE_BURN, FurnaceBurnEvent::new);
        register(ItemEnchantEvent.class, FalconAbi.EVENT_ITEM_ENCHANT, ItemEnchantEvent::new);
        register(ItemDamageEvent.class, FalconAbi.EVENT_ITEM_DAMAGE, ItemDamageEvent::new);
        register(ItemBreakEvent.class, FalconAbi.EVENT_ITEM_BREAK, ItemBreakEvent::new);
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
