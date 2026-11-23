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
/* PLAYER_JOIN fires before the join message goes out and PLAYER_QUIT before the leave message: eventMessage and
   eventSetMessage read and replace it, "" sends none. PLAYER_QUIT's eventCause is why the player left. */
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
#define FALCON_EVENT_PLAYER_TELEPORT 70u
#define FALCON_EVENT_PLAYER_ITEM_USE 71u
#define FALCON_EVENT_PLAYER_BUCKET_FILL 72u
#define FALCON_EVENT_PLAYER_BUCKET_EMPTY 73u
#define FALCON_EVENT_PLAYER_FISH 74u
#define FALCON_EVENT_PLAYER_ARMOR_CHANGE 75u
#define FALCON_EVENT_PLAYER_EMOTE 76u
#define FALCON_EVENT_ENTITY_EFFECT_ADD 77u
#define FALCON_EVENT_ENTITY_HEAL 78u
#define FALCON_EVENT_ENTITY_BREED 79u
#define FALCON_EVENT_ENTITY_TAME 80u
#define FALCON_EVENT_ENTITY_MOUNT 81u
#define FALCON_EVENT_ENTITY_DISMOUNT 82u
#define FALCON_EVENT_ENTITY_TELEPORT 83u
#define FALCON_EVENT_HOPPER_TRANSFER 84u
#define FALCON_EVENT_BLOCK_DISPENSE 85u
#define FALCON_EVENT_BREW 86u
#define FALCON_EVENT_ANVIL_USE 87u
#define FALCON_EVENT_ITEM_SPAWN 88u
#define FALCON_EVENT_PORTAL_CREATE 89u
#define FALCON_EVENT_WORLD_SAVE 90u

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

typedef uint32_t FalconDifficulty;
#define FALCON_DIFFICULTY_PEACEFUL 0u
#define FALCON_DIFFICULTY_EASY 1u
#define FALCON_DIFFICULTY_NORMAL 2u
#define FALCON_DIFFICULTY_HARD 3u

typedef uint32_t FalconBlockFace;
#define FALCON_FACE_DOWN 0u
#define FALCON_FACE_UP 1u
#define FALCON_FACE_NORTH 2u
#define FALCON_FACE_SOUTH 3u
#define FALCON_FACE_WEST 4u
#define FALCON_FACE_EAST 5u

typedef uint32_t FalconMirror;
#define FALCON_MIRROR_NONE 0u
#define FALCON_MIRROR_X 1u
#define FALCON_MIRROR_Z 2u
#define FALCON_MIRROR_XZ 3u

typedef uint32_t FalconEquipmentSlot;
#define FALCON_EQUIPMENT_MAINHAND 0u
#define FALCON_EQUIPMENT_OFFHAND 1u
#define FALCON_EQUIPMENT_HEAD 2u
#define FALCON_EQUIPMENT_CHEST 3u
#define FALCON_EQUIPMENT_LEGS 4u
#define FALCON_EQUIPMENT_FEET 5u
#define FALCON_EQUIPMENT_BODY 6u

typedef uint32_t FalconCameraEase;
#define FALCON_CAMERA_EASE_LINEAR 0u
#define FALCON_CAMERA_EASE_SPRING 1u
#define FALCON_CAMERA_EASE_IN_SINE 2u
#define FALCON_CAMERA_EASE_OUT_SINE 3u
#define FALCON_CAMERA_EASE_IN_OUT_SINE 4u
#define FALCON_CAMERA_EASE_IN_QUAD 5u
#define FALCON_CAMERA_EASE_OUT_QUAD 6u
#define FALCON_CAMERA_EASE_IN_OUT_QUAD 7u
#define FALCON_CAMERA_EASE_IN_CUBIC 8u
#define FALCON_CAMERA_EASE_OUT_CUBIC 9u
#define FALCON_CAMERA_EASE_IN_OUT_CUBIC 10u
#define FALCON_CAMERA_EASE_IN_QUART 11u
#define FALCON_CAMERA_EASE_OUT_QUART 12u
#define FALCON_CAMERA_EASE_IN_OUT_QUART 13u
#define FALCON_CAMERA_EASE_IN_QUINT 14u
#define FALCON_CAMERA_EASE_OUT_QUINT 15u
#define FALCON_CAMERA_EASE_IN_OUT_QUINT 16u
#define FALCON_CAMERA_EASE_IN_EXPO 17u
#define FALCON_CAMERA_EASE_OUT_EXPO 18u
#define FALCON_CAMERA_EASE_IN_OUT_EXPO 19u
#define FALCON_CAMERA_EASE_IN_CIRC 20u
#define FALCON_CAMERA_EASE_OUT_CIRC 21u
#define FALCON_CAMERA_EASE_IN_OUT_CIRC 22u
#define FALCON_CAMERA_EASE_IN_BACK 23u
#define FALCON_CAMERA_EASE_OUT_BACK 24u
#define FALCON_CAMERA_EASE_IN_OUT_BACK 25u
#define FALCON_CAMERA_EASE_IN_ELASTIC 26u
#define FALCON_CAMERA_EASE_OUT_ELASTIC 27u
#define FALCON_CAMERA_EASE_IN_OUT_ELASTIC 28u
#define FALCON_CAMERA_EASE_IN_BOUNCE 29u
#define FALCON_CAMERA_EASE_OUT_BOUNCE 30u
#define FALCON_CAMERA_EASE_IN_OUT_BOUNCE 31u

typedef uint32_t FalconLogLevel;
#define FALCON_LOG_INFO 0u
#define FALCON_LOG_WARNING 1u
#define FALCON_LOG_ERROR 2u

typedef uint32_t FalconCommandPermission;
#define FALCON_PERMISSION_ANY 0u
#define FALCON_PERMISSION_OPERATOR 1u

typedef uint32_t FalconCommandParamType;
#define FALCON_COMMAND_PARAM_INT 0u
#define FALCON_COMMAND_PARAM_FLOAT 1u
#define FALCON_COMMAND_PARAM_STRING 2u
#define FALCON_COMMAND_PARAM_BOOL 3u
#define FALCON_COMMAND_PARAM_TARGET 4u
#define FALCON_COMMAND_PARAM_POSITION 5u
#define FALCON_COMMAND_PARAM_BLOCK_POSITION 6u
#define FALCON_COMMAND_PARAM_MESSAGE 7u
#define FALCON_COMMAND_PARAM_JSON 8u
#define FALCON_COMMAND_PARAM_ENUM 9u
/* the rest of the line, words and spaces, as a command's default [args] */
#define FALCON_COMMAND_PARAM_RAWTEXT 10u

typedef void (*FalconEventHandler)(FalconEvent *event, void *userData);
typedef const char *(*FalconServiceHandler)(const char *request, void *userData);
typedef void (*FalconTask)(void *userData);
typedef int (*FalconCommandHandler)(FalconCommandSender *sender, const char *const *arguments,
                                    uint32_t argumentCount, void *userData);

/* a window a plugin opened: called once when it closes, whoever closed it */
typedef void (*FalconWindowCloseHandler)(FalconPlayer *player, uint32_t window, void *userData);

/* a container screen holding the plugin's own items, with no block behind it: menus and virtual chests. The client
   draws it from a block or an entity it knows, such as a chest the plugin sent it with playerSendPacket. Moves in it
   fire INVENTORY_TRANSACTION with the "container" source or destination, which can refuse them */
typedef struct FalconWindowDescriptor {
    /* the game's container type: 0 a chest, 6 a dispenser, 7 a dropper, 8 a hopper, 12 a horse */
    uint32_t containerType;
    FalconBlockPos position;
    /* the entity it belongs to, -1 for none */
    int64_t actorUniqueId;
    uint32_t size;
    FalconWindowCloseHandler onClose;
    void *userData;
} FalconWindowDescriptor;

/* what the game knows of an item type; the name is the English one, kept until the server stops */
typedef struct FalconItemInfo {
    const char *name;
    uint32_t maxStackSize;
    /* 0 for an item that takes no wear */
    int32_t maxDurability;
    /* 0 none, 1 sword, 2 shovel, 3 pickaxe, 4 axe, 5 shears, 6 hoe, 7 spear */
    uint32_t toolType;
    /* 0 none, 1 wooden, 2 gold, 3 stone, 4 copper, 5 iron, 6 diamond, 7 netherite */
    uint32_t toolTier;
    int32_t attackDamage;
    /* the FALCON_ARMOR_ slot + 1 for armour, 0 for an item worn nowhere */
    uint32_t armorSlot;
    int32_t armorPoints;
    int32_t armorToughness;
    /* the hunger and saturation eating it gives, 0 for an item that is not food */
    int32_t nutrition;
    float saturation;
} FalconItemInfo;

/* what the game knows of a block type; the name is the English one, kept until the server stops */
typedef struct FalconBlockInfo {
    const char *name;
    /* seconds of mining by hand, about; -1 for a block that cannot be broken */
    float hardness;
    float blastResistance;
    /* the tool that breaks it fastest, as FalconItemInfo.toolType, and the tier it needs to drop anything */
    uint32_t toolType;
    uint32_t toolTier;
    int32_t lightEmission;
    int32_t transparent;
    int32_t solid;
    /* how readily fire catches it and burns it away, 0 for a block that does not burn */
    int32_t flameEncouragement;
    int32_t flammability;
    uint32_t maxStackSize;
} FalconBlockInfo;

typedef struct FalconCommandDescriptor {
    const char *name;
    const char *description;
    const char *usage;
    FalconCommandPermission permission;
    FalconCommandHandler handler;
    void *userData;
} FalconCommandDescriptor;

/* one parameter of a command overload; an enum parameter lists its values, which the client completes */
typedef struct FalconCommandParameter {
    const char *name;
    FalconCommandParamType type;
    int32_t optional;
    const char *const *values;
    uint32_t valueCount;
} FalconCommandParameter;

/* a crafting table recipe; shaped: width x height ingredients row by row, NULL or "" for an empty cell */
typedef struct FalconRecipeDescriptor {
    const char *identifier;
    int32_t shaped;
    uint32_t width;
    uint32_t height;
    const char *const *ingredients;
    uint32_t ingredientCount;
    const char *result;
    uint32_t resultCount;
} FalconRecipeDescriptor;

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

typedef struct FalconBlockHit {
    FalconBlockPos block;
    FalconBlockFace face;
    /* where the ray enters the block */
    FalconVec3 position;
    double distance;
} FalconBlockHit;

/* a box between two corners, the lower one first */
typedef struct FalconBox {
    FalconVec3 min;
    FalconVec3 max;
} FalconBox;

#define FALCON_API_PLAYER_COMPATIBILITY 1
#define FALCON_API_ENTITY_MAX_HEALTH 1

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

    int (*commandAddOverload)(FalconPlugin *plugin, const char *command, const FalconCommandParameter *parameters,
                              uint32_t count);
    int (*serverExecuteCommand)(const char *commandLine);
    int (*playerExecuteCommand)(FalconPlayer *player, const char *commandLine);
    int (*registerRecipe)(FalconPlugin *plugin, const FalconRecipeDescriptor *descriptor);
    int (*removeRecipe)(const char *identifier);

    uint32_t (*levelGameRuleCount)(FalconLevel *level);
    const char *(*levelGameRuleName)(FalconLevel *level, uint32_t index);
    const char *(*levelGameRule)(FalconLevel *level, const char *name);
    int (*levelSetGameRule)(FalconLevel *level, const char *name, const char *value);
    FalconDifficulty (*serverDifficulty)(void);
    int (*serverSetDifficulty)(FalconDifficulty difficulty);
    const char *(*levelBiome)(FalconLevel *level, FalconBlockPos position);
    int (*levelRaycastBlock)(FalconLevel *level, FalconVec3 origin, FalconVec3 direction, double maxDistance,
                             int includeLiquids, int includePassable, FalconBlockHit *out);
    FalconEntity *(*levelRaycastEntity)(FalconLevel *level, FalconVec3 origin, FalconVec3 direction, double maxDistance,
                                        FalconEntity *except, double *distance);
    uint32_t (*levelEntitiesInBox)(FalconLevel *level, FalconVec3 from, FalconVec3 to, FalconEntity **entities,
                                   uint32_t capacity);
    int (*levelPlaceStructure)(FalconLevel *level, const char *structure, FalconBlockPos position, uint32_t rotation,
                               FalconMirror mirror, int includeEntities, float integrity);
    int (*levelAddTickingArea)(FalconLevel *level, const char *name, FalconBlockPos from, FalconBlockPos to,
                               int preload);
    int (*levelRemoveTickingArea)(FalconLevel *level, const char *name);
    uint32_t (*levelTickingAreaCount)(FalconLevel *level);
    const char *(*levelTickingAreaName)(FalconLevel *level, uint32_t index);

    FalconItem *(*entityEquipment)(FalconEntity *entity, FalconEquipmentSlot slot);
    int (*entitySetEquipment)(FalconEntity *entity, FalconEquipmentSlot slot, FalconItem *item);
    FalconEntity *(*entityVehicle)(FalconEntity *entity);
    uint32_t (*entityPassengerCount)(FalconEntity *entity);
    FalconEntity *(*entityPassenger)(FalconEntity *entity, uint32_t index);
    int (*entityMount)(FalconEntity *rider, FalconEntity *vehicle);
    void (*entityDismount)(FalconEntity *rider);
    void (*entityEjectPassengers)(FalconEntity *vehicle);
    uint32_t (*entityTagCount)(FalconEntity *entity);
    const char *(*entityTag)(FalconEntity *entity, uint32_t index);
    int (*entityHasTag)(FalconEntity *entity, const char *tag);
    int (*entityAddTag)(FalconEntity *entity, const char *tag);
    int (*entityRemoveTag)(FalconEntity *entity, const char *tag);
    FalconEntity *(*entityAttackTarget)(FalconEntity *entity);
    int (*entitySetAttackTarget)(FalconEntity *entity, FalconEntity *target);

    int (*playerSpawnPoint)(FalconPlayer *player, FalconBlockPos *position, FalconDimension *dimension);
    int (*playerSetSpawnPoint)(FalconPlayer *player, FalconLevel *level, FalconBlockPos position);
    void (*playerClearSpawnPoint)(FalconPlayer *player);
    const char *(*playerLanguage)(FalconPlayer *player);
    int32_t (*playerDeviceOs)(FalconPlayer *player);
    const char *(*playerSkinId)(FalconPlayer *player);
    const uint8_t *(*playerSkinImage)(FalconPlayer *player, uint32_t *width, uint32_t *height, uint32_t *length);
    int (*playerSetCamera)(FalconPlayer *player, const char *preset, const FalconVec3 *position,
                           const FalconVec3 *rotation, float easeTime, FalconCameraEase ease);
    int (*playerFadeCamera)(FalconPlayer *player, float fadeIn, float hold, float fadeOut, uint32_t color);
    void (*playerClearCamera)(FalconPlayer *player);
    int32_t (*playerItemCooldown)(FalconPlayer *player, FalconItem *item);
    int (*playerStartItemCooldown)(FalconPlayer *player, FalconItem *item, int32_t ticks);

    int (*serverBan)(const char *name, const char *reason, const char *source);
    int (*serverPardon)(const char *name);
    int (*serverIsBanned)(const char *name);
    uint32_t (*serverBanCount)(void);
    const char *(*serverBannedName)(uint32_t index);
    int (*serverBanIp)(const char *address, const char *reason, const char *source);
    int (*serverPardonIp)(const char *address);
    int (*serverIsIpBanned)(const char *address);
    uint32_t (*serverIpBanCount)(void);
    const char *(*serverBannedIp)(uint32_t index);
    int (*serverAllowListAdd)(const char *name);
    int (*serverAllowListRemove)(const char *name);
    uint32_t (*serverAllowListCount)(void);
    const char *(*serverAllowListName)(uint32_t index);
    int (*serverAllowListEnabled)(void);
    void (*serverSetAllowListEnabled)(int enabled);
    const char *(*serverMotd)(void);
    void (*serverSetMotd)(const char *motd);
    uint32_t (*serverMaxPlayers)(void);
    int (*serverSetMaxPlayers)(uint32_t maxPlayers);
    double (*serverTps)(void);
    double (*serverMillisecondsPerTick)(void);
    void (*serverShutdown)(void);
    uint32_t (*serverPluginCount)(void);
    const char *(*serverPluginName)(uint32_t index);
    const char *(*serverPluginVersion)(const char *name);
    int (*serverPluginEnabled)(const char *name);

    int (*eventEffect)(FalconEvent *event, FalconEffect *out);

    int (*entitySetNbt)(FalconEntity *entity, const uint8_t *data, uint32_t length);
    int (*playerSetSkin)(FalconPlayer *player, const char *skinId, const uint8_t *rgba, uint32_t length,
                         uint32_t width, uint32_t height);

    FalconPlayer *(*serverSpawnSimulatedPlayer)(const char *name, FalconLevel *level, FalconVec3 position, float yaw,
                                               FalconGameMode gameMode);
    int (*playerIsSimulated)(FalconPlayer *player);
    int (*playerRemoveSimulated)(FalconPlayer *player);

    int64_t (*playerUniqueId)(FalconPlayer *player);

    /* PLAYER_JOIN and PLAYER_QUIT: 1 while the message is still the game's own, which each client translates;
       eventMessage gives it in English */
    int (*eventMessageIsDefault)(FalconEvent *event);
    /* the other names a command of this plugin answers to, in place of the ones set before */
    int (*commandSetAliases)(FalconPlugin *plugin, const char *command, const char *const *aliases, uint32_t count);
    /* says one line in the public chat as the player, as if they typed it: the chat events fire; 0 when the player
       is not in the world or the line is empty */
    int (*playerChat)(FalconPlayer *player, const char *message);

    /* the entity or player with that runtime id, NULL once it is gone: a handle kept across ticks is checked so */
    FalconEntity *(*serverEntity)(uint64_t runtimeId);

    /* the window's id, 0 while the player has another window open or for a size of 0 */
    uint32_t (*playerOpenWindow)(FalconPlugin *plugin, FalconPlayer *player, const FalconWindowDescriptor *descriptor);
    /* a copy of what the slot holds now, NULL once the window closed */
    FalconItem *(*playerWindowItem)(FalconPlayer *player, uint32_t window, uint32_t slot);
    int (*playerSetWindowItem)(FalconPlayer *player, uint32_t window, uint32_t slot, FalconItem *item);
    void (*playerCloseWindow)(FalconPlayer *player, uint32_t window);
    /* the id clients know a block state by, for packets a plugin builds such as UpdateBlock; 0 for an unknown block */
    uint32_t (*serverBlockNetworkId)(const char *name, const char *statesJson);

    /* 0 for an unknown identifier */
    int (*serverItemInfo)(const char *identifier, FalconItemInfo *out);
    int (*serverBlockInfo)(const char *identifier, FalconBlockInfo *out);

    /* BLOCK_BREAK, ENTITY_DEATH and PLAYER_DEATH: the items that will drop, borrowed for the event; a death with
       the keepInventory game rule drops none */
    uint32_t (*eventDropCount)(FalconEvent *event);
    FalconItem *(*eventDropAt)(FalconEvent *event, uint32_t index);
    /* what drops in their place, copied; NULL entries are skipped */
    void (*eventSetDrops)(FalconEvent *event, FalconItem *const *items, uint32_t count);
    /* the same events: the experience that will drop */
    int32_t (*eventExperience)(FalconEvent *event);
    void (*eventSetExperience)(FalconEvent *event, int32_t experience);
    /* PLAYER_DEATH: whether the player keeps their items, and their experience, as the keepInventory rule says
       until a plugin changes it */
    int (*eventKeepInventory)(FalconEvent *event);
    void (*eventSetKeepInventory)(FalconEvent *event, int keepInventory);
    int (*eventKeepExperience)(FalconEvent *event);
    void (*eventSetKeepExperience)(FalconEvent *event, int keepExperience);
    /* ENTITY_DAMAGE: the push the hit gives, horizontal and vertical; 0 for damage that pushes nothing, which
       setting does not change */
    int (*eventKnockback)(FalconEvent *event, float *horizontal, float *vertical);
    void (*eventSetKnockback)(FalconEvent *event, float horizontal, float vertical);

    /* an entity of a custom type its plugin shows to clients itself, with packets of its own: Falcon sends no spawn,
       movement or data for it, gives it no physics or AI, moves it only when told and fires no teleport event for
       that. Hits, interactions, projectiles and queries still find it. 0 for an entity of another type */
    int (*entitySetPluginDrawn)(FalconEntity *entity, int pluginDrawn);
    int (*entityIsPluginDrawn)(FalconEntity *entity);
    /* the box of an entity of a custom type, in place of its type's: what hits and projectiles aim at */
    int (*entitySetSize)(FalconEntity *entity, float width, float height);
    /* the boxes of the blocks in the box between two corners that entities collide with; writes at most capacity of
       them and gives how many there are */
    uint32_t (*levelCollisionBoxes)(FalconLevel *level, FalconVec3 from, FalconVec3 to, FalconBox *boxes,
                                    uint32_t capacity);
    /* whether the player's client holds that chunk of the player's level, so an entity in it can be shown */
    int (*playerHasChunk)(FalconPlayer *player, int32_t chunkX, int32_t chunkZ);
    /* the id clients know the item by, 0 for air, and in blockNetworkId the one of the block it places, 0 for none:
       for packets a plugin builds that carry items, such as MobEquipment */
    int32_t (*itemNetworkId)(FalconItem *item, int32_t *blockNetworkId);

    /* sends the player the commands it may run again, built anew as when it joined: after its permissions changed.
       DATA_PACKET_SEND fires for it; 0 when there is no spawned client to receive the list */
    int (*playerSendCommands)(FalconPlayer *player);
    /* queues a close after all earlier sends for this player */
    int (*playerCloseForms)(FalconPlayer *player);
    /* changes the overhead name for current and future viewers, without changing the login name */
    int (*playerSetNameTag)(FalconPlayer *player, const char *nameTag);
    /* -1: deny, 0: no explicit decision, 1: grant. Excludes defaults and operator fallback. */
    int (*playerPermissionOverride)(FalconPlayer *player, const char *node);
    /* average round-trip time in milliseconds; -1 until known */
    int (*playerPing)(FalconPlayer *player);
    /* sets the base maximum health, clamping current health if needed */
    int (*entitySetMaxHealth)(FalconEntity *entity, float maximum);
} FalconServerApi;

typedef int (*FalconPluginEntry)(const FalconServerApi *api, FalconPlugin *plugin, FalconPluginCallbacks *callbacks);

#ifdef __cplusplus
}
#endif

#endif
