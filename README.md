# Falcon Plugin API

The API used to write plugins for [Falcon](https://github.com/Falcon-MC/Falcon), a Minecraft: Bedrock Edition
server written in C++.

Plugins can be written in **C++**, **C#** and **Java**. All three sit on top of the same stable C ABI
(`native/include/falcon/falcon_api.h`), so a feature added to the API is available in every language.

| Language | Status | Location |
|----------|--------|----------|
| C++      | In development | `native/` |
| C#       | Planned | `dotnet/` |
| Java     | Planned | `java/` |

## Writing a C++ plugin

```cpp
#include <falcon/Falcon.hpp>

class MyPlugin : public falcon::Plugin {
public:
    bool onEnable() override {
        events().on<falcon::PlayerJoinEvent>([](falcon::PlayerJoinEvent &event) {
            event.player().sendMessage("Welcome!");
        });

        commands().add("hello", "Says hello", "/hello", [](falcon::CommandContext &context) {
            context.reply("Hello!");
            return true;
        }, falcon::CommandPermission::Any);

        return true;
    }
};

FALCON_PLUGIN(MyPlugin)
```

Add the API to your CMake project and build a shared library:

```cmake
include(FetchContent)
FetchContent_Declare(FalconPluginAPI GIT_REPOSITORY https://github.com/Falcon-MC/PluginAPI.git GIT_TAG main)
FetchContent_MakeAvailable(FalconPluginAPI)

falcon_add_plugin(MyPlugin MyPlugin.cpp)
```

The C++ API is header-only and only talks to the server through the C ABI: a plugin built with MSVC runs on a
server built with GCC.

## Installing a plugin

```
plugins/
  MyPlugin/
    plugin.json
    MyPlugin.dll        (Windows)
    libMyPlugin.so      (Linux)
    libMyPlugin.dylib   (macOS)
```

`plugin.json`:

```json
{
  "name": "MyPlugin",
  "version": "1.0.0",
  "api-version": "1.0",
  "main": "MyPlugin",
  "runtime": "native",
  "authors": ["you"],
  "description": "What it does",
  "depend": [],
  "softdepend": [],
  "loadbefore": []
}
```

Plugins are loaded when the server starts and unloaded when it stops. There is no hot reloading.

## What the API offers

- **Lifecycle**: `onLoad`, `onEnable`, `onDisable`.
- **Events**: player join, quit and chat, with priorities from `Lowest` to `Monitor`, cancellation and
  `ignoreCancelled`.
- **Commands**: registered with a permission and shown in the client's command list.
- **Scheduler**: delayed and repeating tasks on the main thread, and asynchronous work with a callback on the
  main thread.
- **Logger** and a per-plugin data folder.

See `examples/` for complete plugins.

## Versioning

The API follows semantic versioning. The server refuses a plugin whose major `api-version` differs from its
own. Minor versions only add functions at the end of the server function table.

## License

[LGPL-3.0](LICENSE)
