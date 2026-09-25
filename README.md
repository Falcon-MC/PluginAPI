<p align="center">
	<picture>
		<source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Falcon-MC/Falcon/main/.github/logo-white.png">
		<img src="https://raw.githubusercontent.com/Falcon-MC/Falcon/main/.github/logo.png" alt="Falcon" width="200">
	</picture>
	<br>
	<b>Falcon Plugin API</b>
	<br>
	Plugin API for the Falcon Minecraft: Bedrock Edition server
</p>

<p align="center">
	<a href="https://github.com/Falcon-MC/PluginAPI/actions/workflows/ci.yml"><img src="https://github.com/Falcon-MC/PluginAPI/actions/workflows/ci.yml/badge.svg" alt="CI"></a>
	<img src="https://img.shields.io/badge/api-v1.1-blue" alt="API">
	<img src="https://img.shields.io/badge/language-C%2B%2B17%20%7C%20C%23%20%7C%20Java-00599C" alt="Languages">
	<img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Platform">
</p>

## What is this?

The API plugins use to extend [Falcon](https://github.com/Falcon-MC/Falcon). Every language sits on top of
the same stable C ABI, so the server exposes each feature once and all plugins get it.

- **`native/`** - the C ABI (`falcon_api.h`) and header-only C++ classes built on top of it
- **`internal/`** - support for internal plugins, which use the server's own C++ classes
- **`dotnet/`** - C# bindings, planned
- **`java/`** - Java bindings, planned
- **`examples/`** - complete plugins, built by CI on every supported platform

The C++ headers only talk to the server through the C ABI, so a plugin built with any compiler runs on
any Falcon build of the same API major version.

### Native and internal plugins

| | Native (`"runtime": "native"`) | Internal (`"runtime": "internal"`) |
|---|---|---|
| Talks to the server through | the C ABI | the server's own headers and classes |
| Can do | what the API exposes | anything a fork of Falcon can do |
| Runs on | any server with the same API major version | only the exact server build it was compiled against |
| Compiler | any | the one that built the server, same version |

An internal plugin includes headers such as `Block/Block.h` or `Actor/Mob/MobActor.h`, subclasses `Block`,
`Item`, `MobActor`, `Goal` or `Command`, registers them with `FALCON_REGISTER_BLOCK`, `FALCON_REGISTER_ITEM`
and `FALCON_REGISTER_ACTOR`, and calls any public function of the server. A lower priority number overrides a
vanilla class.

The server refuses an internal plugin built against another commit, build number or compiler, and logs why.
Rebuild the plugin for every server update. Keep its classes in a namespace so their names never collide
with the server's.

## Usage

The API is a header-only CMake target named `Falcon::PluginAPI`. `falcon_add_plugin` creates the shared
library with the right settings:

```cmake
include(FetchContent)

FetchContent_Declare(
    falcon_plugin_api
    GIT_REPOSITORY https://github.com/Falcon-MC/PluginAPI.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(falcon_plugin_api)

falcon_add_plugin(MyPlugin MyPlugin.cpp)
```

```cpp
#include <falcon/Falcon.hpp>

class MyPlugin : public falcon::Plugin {
public:
    bool onEnable() override {
        events().on<falcon::PlayerJoinEvent>([](falcon::PlayerJoinEvent &event) {
            event.player().sendMessage("Welcome!");
        });
        return true;
    }
};

FALCON_PLUGIN(MyPlugin)
```

Put the library next to a `plugin.json` in `plugins/<name>/` on the server:

```json
{
  "name": "MyPlugin",
  "version": "1.0.0",
  "api-version": "1.0",
  "main": "MyPlugin"
}
```

### Internal plugins

`falcon_add_internal_plugin` builds the library against one server release. It adds the server's include
directories, links against the server executable and compiles in the server's build identity. Every Falcon
release publishes the matching SDK, `FalconSDK-<platform>.tar.gz`:

```cmake
falcon_add_internal_plugin(MyPlugin MyPlugin.cpp)
```

```cpp
#include "Block/BlockClassRegistry.h"
#include "Block/Blocks/SaplingBlock.h"

#include <falcon/internal/InternalPlugin.hpp>

namespace myplugin {
    class CustomSaplingBlock : public SaplingBlock {
    public:
        using SaplingBlock::SaplingBlock;

        static bool matches(const std::string &identifier) {
            return SaplingBlock::matches(identifier);
        }
    };

    FALCON_REGISTER_BLOCK(CustomSaplingBlock, 10);

    class MyPlugin : public falcon::internal::Plugin {
    public:
        bool onEnable() override {
            subscribe(getServer().getEventBus().after().mPlayerJoin, [](PlayerJoinAfterEvent &event) {
                event.mPlayer.sendMessage("Welcome!");
            });
            return true;
        }
    };
}

FALCON_INTERNAL_PLUGIN(myplugin::MyPlugin)
```

The manifest uses the `internal` runtime and needs no `api-version`:

```json
{
  "name": "MyPlugin",
  "version": "1.0.0",
  "main": "MyPlugin",
  "runtime": "internal"
}
```

Register classes with the macros or in `onLoad`: the server applies them after every plugin has loaded, and
removes them when the plugin is disabled. The library itself stays loaded until the server exits.

On Windows the plugin imports its symbols from `FalconServer.exe`, so the server executable must keep that
name.

## Building

Only CMake 3.16+ and a C++17 compiler are required. The example plugins are built when the project is the
top level project (`FALCON_PLUGIN_API_BUILD_EXAMPLES`).

```
cmake -B build -G Ninja
cmake --build build
```

Internal plugins are built against one server release, with the compiler named in the SDK's `README.txt`.
Extract the `FalconSDK-<platform>.tar.gz` of that release and point `FALCON_SDK_DIR` at it:

```
cmake -B build -G Ninja -DFALCON_SDK_DIR=/path/to/FalconSDK-linux -DFALCON_PLUGIN_API_BUILD_INTERNAL_EXAMPLES=ON
cmake --build build
```

For a server built from source with `FALCON_EXPORT_SYMBOLS` (on by default), point `FALCON_BUILD_DIR` at its
build directory instead, or build the plugins inside the server's build tree with
`-DFALCON_BUILD_INTERNAL_PLUGINS=ON`.

## Related repositories

- [Falcon](https://github.com/Falcon-MC/Falcon) - the server
- [Protocol](https://github.com/Falcon-MC/Protocol) - packets and network types
- [Network](https://github.com/Falcon-MC/Network) - RakNet and NetherNet transport

## Licensing information

Falcon Plugin API is licensed under the [GNU Lesser General Public License v3.0](LICENSE), which supplements
the [GNU General Public License v3.0](COPYING). Plugins can use it under any license, as long as changes to
the API itself stay under the same license.

Falcon is not affiliated with Mojang. All brands and trademarks belong to their respective owners.
