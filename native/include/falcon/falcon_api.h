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
#define FALCON_PLUGIN_ENTRY_NAME "falcon_plugin_entry"

typedef struct FalconPlugin FalconPlugin;
typedef struct FalconPlayer FalconPlayer;
typedef struct FalconEvent FalconEvent;
typedef struct FalconCommandSender FalconCommandSender;

typedef uint32_t FalconEventType;
#define FALCON_EVENT_PLAYER_JOIN 1u
#define FALCON_EVENT_PLAYER_QUIT 2u
#define FALCON_EVENT_PLAYER_CHAT 3u

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
} FalconServerApi;

typedef int (*FalconPluginEntry)(const FalconServerApi *api, FalconPlugin *plugin, FalconPluginCallbacks *callbacks);

#ifdef __cplusplus
}
#endif

#endif
