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
#define FALCON_API_VERSION_MINOR 2
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
} FalconServerApi;

typedef int (*FalconPluginEntry)(const FalconServerApi *api, FalconPlugin *plugin, FalconPluginCallbacks *callbacks);

#ifdef __cplusplus
}
#endif

#endif
