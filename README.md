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
	<img src="https://img.shields.io/badge/api-v1.0-blue" alt="API">
	<img src="https://img.shields.io/badge/language-C%2B%2B17%20%7C%20C%23%20%7C%20Java-00599C" alt="Languages">
	<img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Platform">
</p>

## What is this?

The API plugins use to extend [Falcon](https://github.com/Falcon-MC/Falcon). Every language sits on top of
the same stable C ABI, so the server exposes each feature once and all plugins get it.

- **`native/`** - the C ABI (`falcon_api.h`) and header-only C++ classes built on top of it
- **`dotnet/`** - C# bindings, planned
- **`java/`** - Java bindings, planned
- **`examples/`** - complete plugins, built by CI on every supported platform

The C++ headers only talk to the server through the C ABI, so a plugin built with any compiler runs on
any Falcon build of the same API major version.

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

## Building

Only CMake 3.16+ and a C++17 compiler are required. The example plugins are built when the project is the
top level project (`FALCON_PLUGIN_API_BUILD_EXAMPLES`).

```
cmake -B build -G Ninja
cmake --build build
```

## Related repositories

- [Falcon](https://github.com/Falcon-MC/Falcon) - the server
- [Protocol](https://github.com/Falcon-MC/Protocol) - packets and network types
- [Network](https://github.com/Falcon-MC/Network) - RakNet and NetherNet transport

## Licensing information

Falcon Plugin API is licensed under the [GNU Lesser General Public License v3.0](LICENSE), which supplements
the [GNU General Public License v3.0](COPYING). Plugins can use it under any license, as long as changes to
the API itself stay under the same license.

Falcon is not affiliated with Mojang. All brands and trademarks belong to their respective owners.
