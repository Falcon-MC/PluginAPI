#ifndef FALCON_API_H
#define FALCON_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#define FALCON_EXPORT __declspec(dllexport)
#else
#define FALCON_EXPORT __attribute__((visibility("default")))
#endif

#define FALCON_API_VERSION_MAJOR 1
#define FALCON_API_VERSION_MINOR 0
#define FALCON_API_VERSION_PATCH 6
#define FALCON_PLUGIN_ENTRY_NAME "falcon_plugin_entry"

typedef struct FalconPlugin FalconPlugin;
typedef struct FalconPlayer FalconPlayer;
typedef struct FalconEvent FalconEvent;
typedef struct FalconCommandSender FalconCommandSender;
typedef struct FalconEntity FalconEntity;
typedef struct FalconLevel FalconLevel;
typedef struct FalconItem FalconItem;

typedef struct FalconVec3 {
    double x;
    double y;
    double z;
} FalconVec3;

typedef struct FalconBlockPos {
    int32_t x;
    int32_t y;
    int32_t z;
} FalconBlockPos;

typedef uint32_t FalconEventType;
#define FALCON_EVENT_PLAYER_JOIN 1u
#define FALCON_EVENT_PLAYER_QUIT 2u
#define FALCON_EVENT_PLAYER_CHAT 3u
#define FALCON_EVENT_BLOCK_BREAK 4u
#define FALCON_EVENT_BLOCK_PLACE 5u
#define FALCON_EVENT_PLAYER_INTERACT_BLOCK 6u
#define FALCON_EVENT_ENTITY_DAMAGE 7u
#define FALCON_EVENT_ENTITY_DEATH 8u
#define FALCON_EVENT_PLAYER_DEATH 9u
#define FALCON_EVENT_PLAYER_RESPAWN 10u
#define FALCON_EVENT_PLAYER_MOVE 11u
#define FALCON_EVENT_PLAYER_DROP_ITEM 12u
#define FALCON_EVENT_PLAYER_PICKUP_ITEM 13u
#define FALCON_EVENT_DATA_PACKET_RECEIVE 14u
#define FALCON_EVENT_DATA_PACKET_SEND 15u
#define FALCON_EVENT_PLAYER_INTERACT_ENTITY 16u
#define FALCON_EVENT_INVENTORY_OPEN 17u
#define FALCON_EVENT_INVENTORY_CLOSE 18u
#define FALCON_EVENT_INVENTORY_TRANSACTION 19u
#define FALCON_EVENT_CRAFT_ITEM 20u
#define FALCON_EVENT_PLAYER_COMMAND 21u
#define FALCON_EVENT_SERVER_COMMAND 22u
#define FALCON_EVENT_SERVER_TICK 23u
#define FALCON_EVENT_PLAYER_GAME_MODE_CHANGE 24u
#define FALCON_EVENT_PLAYER_CHANGE_DIMENSION 25u
#define FALCON_EVENT_PROJECTILE_HIT 26u
#define FALCON_EVENT_EXPLOSION 27u
#define FALCON_EVENT_FIRE_SPREAD 28u
#define FALCON_EVENT_BLOCK_BURN 29u
#define FALCON_EVENT_CUSTOM 30u
#define FALCON_EVENT_PLAYER_PRE_LOGIN 31u
#define FALCON_EVENT_PLAYER_TOGGLE_SNEAK 32u
#define FALCON_EVENT_PLAYER_TOGGLE_SPRINT 33u
#define FALCON_EVENT_PLAYER_TOGGLE_FLIGHT 34u
#define FALCON_EVENT_PLAYER_ITEM_HELD 35u
#define FALCON_EVENT_PLAYER_ITEM_CONSUME 36u
#define FALCON_EVENT_PLAYER_BED_ENTER 37u
#define FALCON_EVENT_PLAYER_BED_LEAVE 38u
#define FALCON_EVENT_PLAYER_JUMP 39u
#define FALCON_EVENT_PLAYER_FOOD_CHANGE 40u
#define FALCON_EVENT_PLAYER_EXPERIENCE_CHANGE 41u
#define FALCON_EVENT_ENTITY_SPAWN 42u
#define FALCON_EVENT_ENTITY_TRANSFORM 43u
#define FALCON_EVENT_ENTITY_TARGET 44u
#define FALCON_EVENT_PROJECTILE_LAUNCH 45u
#define FALCON_EVENT_BLOCK_GROW 46u
#define FALCON_EVENT_BLOCK_SPREAD 47u
#define FALCON_EVENT_BLOCK_FORM 48u
#define FALCON_EVENT_BLOCK_FADE 49u
#define FALCON_EVENT_LIQUID_FLOW 50u
#define FALCON_EVENT_PISTON_EXTEND 51u
#define FALCON_EVENT_PISTON_RETRACT 52u
#define FALCON_EVENT_REDSTONE_CHANGE 53u
#define FALCON_EVENT_SIGN_CHANGE 54u
#define FALCON_EVENT_LEAVES_DECAY 55u
#define FALCON_EVENT_WEATHER_CHANGE 56u
#define FALCON_EVENT_THUNDER_CHANGE 57u
#define FALCON_EVENT_CHUNK_LOAD 58u
#define FALCON_EVENT_CHUNK_UNLOAD 59u
#define FALCON_EVENT_FURNACE_SMELT 60u
#define FALCON_EVENT_FURNACE_BURN 61u
#define FALCON_EVENT_ITEM_ENCHANT 62u
#define FALCON_EVENT_ITEM_DAMAGE 63u
#define FALCON_EVENT_ITEM_BREAK 64u
#define FALCON_EVENT_WORLD_LOAD 65u
#define FALCON_EVENT_WORLD_UNLOAD 66u
#define FALCON_EVENT_PLAYER_CHANGE_WORLD 67u
#define FALCON_EVENT_CONNECTION_OPEN 68u
#define FALCON_EVENT_PLAYER_FORM_RESPONSE 69u

typedef uint32_t FalconGameMode;
#define FALCON_GAME_MODE_SURVIVAL 0u
#define FALCON_GAME_MODE_CREATIVE 1u
#define FALCON_GAME_MODE_ADVENTURE 2u
#define FALCON_GAME_MODE_SPECTATOR 6u

typedef uint32_t FalconDimension;
#define FALCON_DIMENSION_OVERWORLD 0u
#define FALCON_DIMENSION_NETHER 1u
#define FALCON_DIMENSION_THE_END 2u

typedef uint32_t FalconPermissionDefault;
#define FALCON_PERMISSION_DEFAULT_FALSE 0u
#define FALCON_PERMISSION_DEFAULT_TRUE 1u
#define FALCON_PERMISSION_DEFAULT_OPERATOR 2u

typedef uint32_t FalconArmorSlot;
#define FALCON_ARMOR_HEAD 0u
#define FALCON_ARMOR_CHEST 1u
#define FALCON_ARMOR_LEGS 2u
#define FALCON_ARMOR_FEET 3u

typedef uint32_t FalconEventPriority;
#define FALCON_PRIORITY_LOWEST 0u
#define FALCON_PRIORITY_LOW 1u
#define FALCON_PRIORITY_NORMAL 2u
#define FALCON_PRIORITY_HIGH 3u
#define FALCON_PRIORITY_HIGHEST 4u
#define FALCON_PRIORITY_MONITOR 5u

typedef uint32_t FalconImageFit;
#define FALCON_IMAGE_FIT_STRETCH 0u
#define FALCON_IMAGE_FIT_CONTAIN 1u
#define FALCON_IMAGE_FIT_COVER 2u

#define FALCON_MAP_SIZE 128u

typedef uint32_t FalconEffectType;
#define FALCON_EFFECT_SPEED 1u
#define FALCON_EFFECT_SLOWNESS 2u
#define FALCON_EFFECT_HASTE 3u
#define FALCON_EFFECT_MINING_FATIGUE 4u
#define FALCON_EFFECT_STRENGTH 5u
#define FALCON_EFFECT_INSTANT_HEALTH 6u
#define FALCON_EFFECT_INSTANT_DAMAGE 7u
#define FALCON_EFFECT_JUMP_BOOST 8u
#define FALCON_EFFECT_NAUSEA 9u
#define FALCON_EFFECT_REGENERATION 10u
#define FALCON_EFFECT_RESISTANCE 11u
#define FALCON_EFFECT_FIRE_RESISTANCE 12u
#define FALCON_EFFECT_WATER_BREATHING 13u
#define FALCON_EFFECT_INVISIBILITY 14u
#define FALCON_EFFECT_BLINDNESS 15u
#define FALCON_EFFECT_NIGHT_VISION 16u
#define FALCON_EFFECT_HUNGER 17u
#define FALCON_EFFECT_WEAKNESS 18u
#define FALCON_EFFECT_POISON 19u
#define FALCON_EFFECT_WITHER 20u
#define FALCON_EFFECT_HEALTH_BOOST 21u
#define FALCON_EFFECT_ABSORPTION 22u
#define FALCON_EFFECT_SATURATION 23u
#define FALCON_EFFECT_LEVITATION 24u
#define FALCON_EFFECT_FATAL_POISON 25u
#define FALCON_EFFECT_CONDUIT_POWER 26u
#define FALCON_EFFECT_SLOW_FALLING 27u
#define FALCON_EFFECT_BAD_OMEN 28u
#define FALCON_EFFECT_VILLAGE_HERO 29u
#define FALCON_EFFECT_DARKNESS 30u
#define FALCON_EFFECT_WEAVING 33u
#define FALCON_EFFECT_RAID_OMEN 36u
#define FALCON_EFFECT_FREEZING 38u

#define FALCON_EFFECT_INFINITE -1

typedef uint32_t FalconDisplaySlot;
#define FALCON_DISPLAY_SLOT_SIDEBAR 0u
#define FALCON_DISPLAY_SLOT_LIST 1u
#define FALCON_DISPLAY_SLOT_BELOW_NAME 2u

typedef uint32_t FalconSignSide;
#define FALCON_SIGN_FRONT 0u
#define FALCON_SIGN_BACK 1u

typedef uint32_t FalconLogLevel;
#define FALCON_LOG_INFO 0u
#define FALCON_LOG_WARNING 1u
#define FALCON_LOG_ERROR 2u

typedef uint32_t FalconCommandPermission;
#define FALCON_PERMISSION_ANY 0u
#define FALCON_PERMISSION_OPERATOR 1u

typedef void (*FalconEventHandler)(FalconEvent *event, void *userData);
typedef const char *(*FalconServiceHandler)(const char *request, void *userData);
typedef void (*FalconTask)(void *userData);
typedef int (*FalconCommandHandler)(FalconCommandSender *sender, const char *const *arguments,
                                    uint32_t argumentCount, void *userData);

typedef struct FalconCommandDescriptor {
    const char *name;
    const char *description;
    const char *usage;
    FalconCommandPermission permission;
    FalconCommandHandler handler;
    void *userData;
} FalconCommandDescriptor;

typedef int (*FalconItemUseHandler)(FalconPlayer *player, FalconItem *item, void *userData);
typedef int (*FalconItemUseOnBlockHandler)(FalconPlayer *player, FalconItem *item, FalconBlockPos position,
                                           uint32_t face, void *userData);
typedef int (*FalconBlockInteractHandler)(FalconPlayer *player, FalconBlockPos position, uint32_t face,
                                          void *userData);
typedef void (*FalconBlockBreakHandler)(FalconPlayer *player, FalconBlockPos position, void *userData);
typedef void (*FalconEntityTickHandler)(FalconEntity *entity, void *userData);
typedef int (*FalconEntityInteractHandler)(FalconEntity *entity, FalconPlayer *player, void *userData);

typedef struct FalconCustomItemDescriptor {
    const char *identifier;
    const char *displayName;
    const char *icon;
    const char *creativeCategory;
    uint32_t maxStackSize;
    uint32_t maxDurability;
    int32_t handEquipped;
    FalconItemUseHandler onUse;
    FalconItemUseOnBlockHandler onUseOnBlock;
    void *userData;
} FalconCustomItemDescriptor;

typedef struct FalconCustomBlockDescriptor {
    const char *identifier;
    const char *displayName;
    const char *texture;
    const char *creativeCategory;
    float destroyTime;
    float explosionResistance;
    uint32_t lightEmission;
    float friction;
    const char *drop;
    FalconBlockInteractHandler onInteract;
    FalconBlockBreakHandler onBreak;
    void *userData;
} FalconCustomBlockDescriptor;

typedef struct FalconCustomEntityDescriptor {
    const char *identifier;
    float width;
    float height;
    float maxHealth;
    int32_t summonable;
    FalconEntityTickHandler onTick;
    FalconEntityInteractHandler onInteract;
    void *userData;
} FalconCustomEntityDescriptor;

typedef struct FalconPluginCallbacks {
    void (*onLoad)(void *userData);
    int (*onEnable)(void *userData);
    void (*onDisable)(void *userData);
    void *userData;
} FalconPluginCallbacks;

typedef struct FalconEffect {
    FalconEffectType type;
    /* ticks left, FALCON_EFFECT_INFINITE for an endless effect */
    int32_t duration;
    int32_t amplifier;
    int32_t showParticles;
    int32_t ambient;
} FalconEffect;

typedef struct FalconAttribute {
    float value;
    float minimum;
    float maximum;
    float defaultValue;
} FalconAttribute;

typedef struct FalconServerApi {
    uint32_t size;
    uint32_t versionMajor;
    uint32_t versionMinor;

    const char *(*serverVersion)(void);
    void (*log)(FalconPlugin *plugin, FalconLogLevel level, const char *message);
    const char *(*pluginName)(FalconPlugin *plugin);
    const char *(*pluginDataFolder)(FalconPlugin *plugin);

    uint32_t (*onlinePlayerCount)(void);
    FalconPlayer *(*onlinePlayer)(uint32_t index);
    FalconPlayer *(*findPlayer)(const char *name);
    const char *(*playerName)(FalconPlayer *player);
    int (*playerIsOperator)(FalconPlayer *player);
    void (*playerSendMessage)(FalconPlayer *player, const char *message);
    void (*playerKick)(FalconPlayer *player, const char *reason);
    void (*broadcastMessage)(const char *message);

    uint64_t (*subscribe)(FalconPlugin *plugin, FalconEventType type, FalconEventPriority priority,
                          int ignoreCancelled, FalconEventHandler handler, void *userData);
    void (*unsubscribe)(uint64_t subscription);
    FalconEventType (*eventType)(FalconEvent *event);
    int (*eventIsCancellable)(FalconEvent *event);
    int (*eventIsCancelled)(FalconEvent *event);
    void (*eventSetCancelled)(FalconEvent *event, int cancelled);
    FalconPlayer *(*eventPlayer)(FalconEvent *event);
    const char *(*eventMessage)(FalconEvent *event);
    void (*eventSetMessage)(FalconEvent *event, const char *message);

    int (*registerCommand)(FalconPlugin *plugin, const FalconCommandDescriptor *descriptor);
    const char *(*senderName)(FalconCommandSender *sender);
    FalconPlayer *(*senderPlayer)(FalconCommandSender *sender);
    void (*senderSendMessage)(FalconCommandSender *sender, const char *message);

    uint64_t (*scheduleTask)(FalconPlugin *plugin, FalconTask task, void *userData, uint64_t delayTicks,
                             uint64_t periodTicks);
    uint64_t (*runAsync)(FalconPlugin *plugin, FalconTask work, FalconTask done, void *userData);
    void (*cancelTask)(uint64_t task);

    FalconEntity *(*eventEntity)(FalconEvent *event);
    FalconEntity *(*eventAttacker)(FalconEvent *event);
    FalconBlockPos (*eventBlockPosition)(FalconEvent *event);
    uint32_t (*eventBlockFace)(FalconEvent *event);
    const char *(*eventBlockName)(FalconEvent *event);
    double (*eventAmount)(FalconEvent *event);
    void (*eventSetAmount)(FalconEvent *event, double amount);
    const char *(*eventCause)(FalconEvent *event);
    FalconVec3 (*eventFrom)(FalconEvent *event);
    FalconVec3 (*eventTo)(FalconEvent *event);
    void (*eventSetTo)(FalconEvent *event, FalconVec3 position);
    FalconItem *(*eventItem)(FalconEvent *event);
    uint32_t (*eventPacketId)(FalconEvent *event);
    const uint8_t *(*eventPacketData)(FalconEvent *event, uint32_t *length);
    void (*eventSetPacketData)(FalconEvent *event, const uint8_t *data, uint32_t length);

    FalconEntity *(*playerEntity)(FalconPlayer *player);
    FalconPlayer *(*entityPlayer)(FalconEntity *entity);
    const char *(*entityType)(FalconEntity *entity);
    uint64_t (*entityRuntimeId)(FalconEntity *entity);
    FalconLevel *(*entityLevel)(FalconEntity *entity);
    FalconVec3 (*entityPosition)(FalconEntity *entity);
    FalconVec3 (*entityRotation)(FalconEntity *entity);
    void (*entityTeleport)(FalconEntity *entity, FalconLevel *level, FalconVec3 position);
    FalconVec3 (*entityMotion)(FalconEntity *entity);
    void (*entitySetMotion)(FalconEntity *entity, FalconVec3 motion);
    float (*entityHealth)(FalconEntity *entity);
    float (*entityMaxHealth)(FalconEntity *entity);
    void (*entitySetHealth)(FalconEntity *entity, float health);
    int (*entityIsAlive)(FalconEntity *entity);
    int (*entityDamage)(FalconEntity *entity, float amount, const char *cause, FalconEntity *attacker);
    void (*entityKill)(FalconEntity *entity);
    void (*entityRemove)(FalconEntity *entity);
    const char *(*entityNameTag)(FalconEntity *entity);
    void (*entitySetNameTag)(FalconEntity *entity, const char *nameTag);
    int (*entityIsOnFire)(FalconEntity *entity);
    void (*entitySetOnFire)(FalconEntity *entity, uint32_t ticks);
    uint32_t (*levelEntityCount)(FalconLevel *level);
    FalconEntity *(*levelEntity)(FalconLevel *level, uint32_t index);

    FalconGameMode (*playerGameMode)(FalconPlayer *player);
    void (*playerSetGameMode)(FalconPlayer *player, FalconGameMode gameMode);
    const char *(*playerXuid)(FalconPlayer *player);
    const char *(*playerUuid)(FalconPlayer *player);
    const char *(*playerAddress)(FalconPlayer *player);
    void (*playerSendTitle)(FalconPlayer *player, const char *title, const char *subtitle);
    void (*playerSendActionBar)(FalconPlayer *player, const char *message);
    float (*playerFood)(FalconPlayer *player);
    void (*playerSetFood)(FalconPlayer *player, float food);
    int32_t (*playerXpLevel)(FalconPlayer *player);
    void (*playerSetXpLevel)(FalconPlayer *player, int32_t level);
    void (*playerSetOperator)(FalconPlayer *player, int operator_);

    FalconLevel *(*serverLevel)(FalconDimension dimension);
    FalconDimension (*levelDimension)(FalconLevel *level);
    const char *(*levelName)(FalconLevel *level);
    const char *(*levelGetBlock)(FalconLevel *level, FalconBlockPos position);
    const char *(*levelGetBlockStates)(FalconLevel *level, FalconBlockPos position);
    int (*levelSetBlock)(FalconLevel *level, FalconBlockPos position, const char *name, const char *statesJson);
    int (*levelBreakBlock)(FalconLevel *level, FalconBlockPos position, int dropItems);
    int (*levelIsChunkLoaded)(FalconLevel *level, int32_t chunkX, int32_t chunkZ);
    int32_t (*levelHighestBlockY)(FalconLevel *level, int32_t x, int32_t z);
    int64_t (*levelTime)(FalconLevel *level);
    void (*levelSetTime)(FalconLevel *level, int64_t time);
    int (*levelIsRaining)(FalconLevel *level);
    void (*levelSetRaining)(FalconLevel *level, int raining);
    int (*levelIsThundering)(FalconLevel *level);
    void (*levelSetThundering)(FalconLevel *level, int thundering);
    FalconVec3 (*levelSpawnPosition)(FalconLevel *level);
    FalconEntity *(*levelSpawnEntity)(FalconLevel *level, const char *identifier, FalconVec3 position);
    void (*levelDropItem)(FalconLevel *level, FalconVec3 position, FalconItem *item);
    void (*levelStrikeLightning)(FalconLevel *level, FalconVec3 position);
    void (*levelCreateExplosion)(FalconLevel *level, FalconVec3 position, float power, int breakBlocks);

    FalconItem *(*itemCreate)(const char *identifier, uint32_t count);
    FalconItem *(*itemCopy)(FalconItem *item);
    void (*itemDestroy)(FalconItem *item);
    int (*itemIsEmpty)(FalconItem *item);
    const char *(*itemIdentifier)(FalconItem *item);
    uint32_t (*itemCount)(FalconItem *item);
    void (*itemSetCount)(FalconItem *item, uint32_t count);
    int32_t (*itemDamage)(FalconItem *item);
    void (*itemSetDamage)(FalconItem *item, int32_t damage);
    const char *(*itemCustomName)(FalconItem *item);
    void (*itemSetCustomName)(FalconItem *item, const char *name);
    uint32_t (*itemLoreCount)(FalconItem *item);
    const char *(*itemLore)(FalconItem *item, uint32_t index);
    void (*itemSetLore)(FalconItem *item, const char *const *lines, uint32_t count);
    int32_t (*itemEnchantmentLevel)(FalconItem *item, uint32_t enchantment);
    void (*itemSetEnchantmentLevel)(FalconItem *item, uint32_t enchantment, int32_t level);
    uint32_t (*playerInventorySize)(FalconPlayer *player);
    FalconItem *(*playerInventoryItem)(FalconPlayer *player, uint32_t slot);
    void (*playerSetInventoryItem)(FalconPlayer *player, uint32_t slot, FalconItem *item);
    int (*playerGiveItem)(FalconPlayer *player, FalconItem *item);
    uint32_t (*playerSelectedSlot)(FalconPlayer *player);
    void (*playerSetSelectedSlot)(FalconPlayer *player, uint32_t slot);
    FalconItem *(*playerArmorItem)(FalconPlayer *player, FalconArmorSlot slot);
    void (*playerSetArmorItem)(FalconPlayer *player, FalconArmorSlot slot, FalconItem *item);
    FalconItem *(*playerOffhandItem)(FalconPlayer *player);
    void (*playerSetOffhandItem)(FalconPlayer *player, FalconItem *item);
    void (*playerClearInventory)(FalconPlayer *player);

    int (*registerPermission)(FalconPlugin *plugin, const char *node, FalconPermissionDefault defaultValue);
    int (*playerHasPermission)(FalconPlayer *player, const char *node);
    void (*playerSetPermission)(FalconPlugin *plugin, FalconPlayer *player, const char *node, int value);
    void (*playerUnsetPermission)(FalconPlugin *plugin, FalconPlayer *player, const char *node);
    int (*senderHasPermission)(FalconCommandSender *sender, const char *node);

    int (*playerSendPacket)(FalconPlayer *player, uint32_t packetId, const uint8_t *data, uint32_t length);

    int (*registerCustomItem)(FalconPlugin *plugin, const FalconCustomItemDescriptor *descriptor);
    int (*registerCustomBlock)(FalconPlugin *plugin, const FalconCustomBlockDescriptor *descriptor);
    int (*registerCustomEntity)(FalconPlugin *plugin, const FalconCustomEntityDescriptor *descriptor);

    FalconLevel *(*eventLevel)(FalconEvent *event);
    FalconEntity *(*eventTarget)(FalconEvent *event);
    FalconVec3 (*eventPosition)(FalconEvent *event);
    uint32_t (*eventBlockCount)(FalconEvent *event);
    FalconBlockPos (*eventBlockAt)(FalconEvent *event, uint32_t index);
    void (*eventSetBlocks)(FalconEvent *event, const FalconBlockPos *positions, uint32_t count);
    FalconGameMode (*eventGameMode)(FalconEvent *event);
    FalconGameMode (*eventPreviousGameMode)(FalconEvent *event);
    FalconDimension (*eventDimension)(FalconEvent *event);
    FalconDimension (*eventPreviousDimension)(FalconEvent *event);
    uint64_t (*eventTick)(FalconEvent *event);
    const char *(*eventSourceContainer)(FalconEvent *event);
    int32_t (*eventSourceSlot)(FalconEvent *event);
    const char *(*eventDestinationContainer)(FalconEvent *event);
    int32_t (*eventDestinationSlot)(FalconEvent *event);

    int (*registerService)(FalconPlugin *plugin, const char *name, FalconServiceHandler handler, void *userData);
    void (*unregisterService)(FalconPlugin *plugin, const char *name);
    int (*hasService)(const char *name);
    const char *(*serviceProvider)(const char *name);
    const char *(*callService)(const char *name, const char *request, int *found);
    uint64_t (*subscribeCustomEvent)(FalconPlugin *plugin, const char *name, FalconEventPriority priority,
                                     int ignoreCancelled, FalconEventHandler handler, void *userData);
    const char *(*fireCustomEvent)(FalconPlugin *plugin, const char *name, const char *data, int cancellable,
                                   int *cancelled);
    const char *(*eventName)(FalconEvent *event);
    const char *(*eventData)(FalconEvent *event);
    void (*eventSetData)(FalconEvent *event, const char *data);

    int (*eventState)(FalconEvent *event);
    double (*eventPreviousAmount)(FalconEvent *event);
    const char *(*eventPreviousBlockName)(FalconEvent *event);
    int32_t (*eventChunkX)(FalconEvent *event);
    int32_t (*eventChunkZ)(FalconEvent *event);
    FalconItem *(*eventResult)(FalconEvent *event);
    void (*eventSetResult)(FalconEvent *event, FalconItem *item);

    uint32_t (*serverWorldCount)(void);
    const char *(*serverWorldName)(uint32_t index);
    const char *(*serverDefaultWorldName)(void);
    FalconLevel *(*worldLevel)(const char *world, FalconDimension dimension);
    int (*serverLoadWorld)(const char *name, int create, const char *seed);
    int (*serverUnloadWorld)(const char *name);
    const char *(*eventWorldName)(FalconEvent *event);
    const char *(*eventPreviousWorldName)(FalconEvent *event);

    const char *(*eventAddress)(FalconEvent *event);
    const char *(*eventTransport)(FalconEvent *event);
    uint64_t (*eventClientGuid)(FalconEvent *event);
    uint32_t (*eventMtuSize)(FalconEvent *event);
    const char *(*eventClientData)(FalconEvent *event);
    const char *(*eventClientDataField)(FalconEvent *event, const char *key);

    uint32_t (*playerSendForm)(FalconPlayer *player, const char *formJson);
    int (*playerTransfer)(FalconPlayer *player, const char *address, uint32_t port);
    uint32_t (*eventFormId)(FalconEvent *event);
    const char *(*eventFormResponse)(FalconEvent *event);
    int (*entityIsInvisible)(FalconEntity *entity);
    void (*entitySetInvisible)(FalconEntity *entity, int invisible);

    int32_t (*playerProtocolVersion)(FalconPlayer *player);

    uint32_t (*playerAddBossBar)(FalconPlayer *player, const char *title, float percent, uint32_t color);
    int (*playerUpdateBossBar)(FalconPlayer *player, uint32_t bar, const char *title, float percent, uint32_t color);
    int (*playerRemoveBossBar)(FalconPlayer *player, uint32_t bar);

    int64_t (*mapCreateImage)(FalconLevel *level, const uint8_t *rgba, uint32_t length);
    uint32_t (*mapCreateImageGrid)(FalconLevel *level, const uint8_t *rgba, uint32_t length, uint32_t width,
                                   uint32_t height, uint32_t columns, uint32_t rows, FalconImageFit fit,
                                   int64_t *mapIds, uint32_t capacity, uint32_t *outColumns, uint32_t *outRows);
    uint32_t (*mapCreateImageGridFromPng)(FalconLevel *level, const uint8_t *png, uint32_t length, uint32_t columns,
                                          uint32_t rows, FalconImageFit fit, int64_t *mapIds, uint32_t capacity,
                                          uint32_t *outColumns, uint32_t *outRows);
    int (*mapPlaceImageGrid)(FalconLevel *level, FalconBlockPos topLeft, uint32_t facing, const int64_t *mapIds,
                             uint32_t count, uint32_t columns, uint32_t rows, int glowing);
    int (*mapUpdateImage)(FalconLevel *level, int64_t mapId, uint32_t x, uint32_t y, uint32_t width,
                          uint32_t height, const uint8_t *rgba, uint32_t length);
    int (*mapIsImage)(FalconLevel *level, int64_t mapId);
    int (*playerGiveMap)(FalconPlayer *player, int64_t mapId);
    const char *(*mapLastError)(void);
    int (*mapDeleteImage)(FalconLevel *level, int64_t mapId);

    int (*entityAddEffect)(FalconEntity *entity, FalconEffectType type, int32_t duration, int32_t amplifier,
                           int showParticles);
    int (*entityRemoveEffect)(FalconEntity *entity, FalconEffectType type);
    int (*entityEffect)(FalconEntity *entity, FalconEffectType type, FalconEffect *out);
    uint32_t (*entityEffectCount)(FalconEntity *entity);
    int (*entityEffectAt)(FalconEntity *entity, uint32_t index, FalconEffect *out);
    void (*entityClearEffects)(FalconEntity *entity);
    uint32_t (*entityAttributeCount)(FalconEntity *entity);
    const char *(*entityAttributeName)(FalconEntity *entity, uint32_t index);
    int (*entityAttribute)(FalconEntity *entity, const char *name, FalconAttribute *out);
    int (*entitySetAttribute)(FalconEntity *entity, const char *name, float value);
    void (*playerPlaySound)(FalconPlayer *player, const char *sound, FalconVec3 position, float volume, float pitch);
    void (*levelPlaySound)(FalconLevel *level, const char *sound, FalconVec3 position, float volume, float pitch);
    void (*playerStopSound)(FalconPlayer *player, const char *sound);
    void (*playerSpawnParticle)(FalconPlayer *player, const char *particle, FalconVec3 position);
    void (*levelSpawnParticle)(FalconLevel *level, const char *particle, FalconVec3 position);

    int (*scoreboardAddObjective)(const char *name, const char *displayName);
    int (*scoreboardRemoveObjective)(const char *name);
    uint32_t (*scoreboardObjectiveCount)(void);
    const char *(*scoreboardObjectiveName)(uint32_t index);
    const char *(*scoreboardObjectiveDisplayName)(const char *objective);
    int (*scoreboardEntityScore)(const char *objective, FalconEntity *entity, int32_t *score);
    int (*scoreboardSetEntityScore)(const char *objective, FalconEntity *entity, int32_t score);
    int (*scoreboardResetEntityScore)(const char *objective, FalconEntity *entity);
    int (*scoreboardFakePlayerScore)(const char *objective, const char *name, int32_t *score);
    int (*scoreboardSetFakePlayerScore)(const char *objective, const char *name, int32_t score);
    int (*scoreboardResetFakePlayerScore)(const char *objective, const char *name);
    uint32_t (*scoreboardScoreCount)(const char *objective);
    const char *(*scoreboardScoreAt)(const char *objective, uint32_t index, int32_t *score);
    FalconEntity *(*scoreboardScoreEntity)(const char *objective, uint32_t index);
    int (*scoreboardSetDisplay)(FalconDisplaySlot slot, const char *objective, int ascending);
    const char *(*scoreboardDisplayObjective)(FalconDisplaySlot slot);

    uint32_t (*levelContainerSize)(FalconLevel *level, FalconBlockPos position);
    FalconItem *(*levelContainerItem)(FalconLevel *level, FalconBlockPos position, uint32_t slot);
    int (*levelSetContainerItem)(FalconLevel *level, FalconBlockPos position, uint32_t slot, FalconItem *item);
    const char *(*levelSignText)(FalconLevel *level, FalconBlockPos position, FalconSignSide side);
    int (*levelSetSignText)(FalconLevel *level, FalconBlockPos position, FalconSignSide side, const char *text);
    uint32_t (*playerEnderChestSize)(FalconPlayer *player);
    FalconItem *(*playerEnderChestItem)(FalconPlayer *player, uint32_t slot);
    int (*playerSetEnderChestItem)(FalconPlayer *player, uint32_t slot, FalconItem *item);

    const char *(*entityData)(FalconPlugin *plugin, FalconEntity *entity, const char *key);
    int (*entitySetData)(FalconPlugin *plugin, FalconEntity *entity, const char *key, const char *value);
    uint32_t (*entityDataCount)(FalconPlugin *plugin, FalconEntity *entity);
    const char *(*entityDataKey)(FalconPlugin *plugin, FalconEntity *entity, uint32_t index);
    const char *(*itemData)(FalconPlugin *plugin, FalconItem *item, const char *key);
    int (*itemSetData)(FalconPlugin *plugin, FalconItem *item, const char *key, const char *value);
    uint32_t (*itemDataCount)(FalconPlugin *plugin, FalconItem *item);
    const char *(*itemDataKey)(FalconPlugin *plugin, FalconItem *item, uint32_t index);
    const uint8_t *(*itemNbt)(FalconItem *item, uint32_t *length);
    int (*itemSetNbt)(FalconItem *item, const uint8_t *data, uint32_t length);
    const uint8_t *(*entityNbt)(FalconEntity *entity, uint32_t *length);
} FalconServerApi;

typedef int (*FalconPluginEntry)(const FalconServerApi *api, FalconPlugin *plugin, FalconPluginCallbacks *callbacks);

#ifdef __cplusplus
}
#endif

#endif
