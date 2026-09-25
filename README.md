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
	<img src="https://img.shields.io/badge/api-v1.2-blue" alt="API">
	<img src="https://img.shields.io/badge/language-C%2B%2B17%20%7C%20C%23%20%7C%20Java-00599C" alt="Languages">
	<img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey" alt="Platform">
</p>

## What is this?

The API plugins use to extend [Falcon](https://github.com/Falcon-MC/Falcon). Every language sits on top of
the same stable C ABI, so the server exposes each feature once and all plugins get it.

- **`native/`** - the C ABI (`falcon_api.h`) and header-only C++ classes built on top of it
- **`internal/`** - support for internal plugins, which use the server's own C++ classes
- **`dotnet/`** - the `Falcon.PluginAPI` package for C# plugins: interop generated from `falcon_api.h` at
  build time, C# classes mirroring the C++ ones, and the loader the server runs through the .NET runtime
- **`java/`** - the `falcon-plugin-api` Maven artifact for Java plugins: bindings generated from `falcon_api.h`
  at build time with the Foreign Function & Memory API, Java classes mirroring the C++ ones, and the loader the
  server runs in its embedded Java virtual machine
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

### Services and custom events

Plugins talk to each other through the server, whatever language they are written in. A plugin provides a
named service and any other plugin calls it with a text request, usually JSON. Custom events work like the
server's events, with priorities and cancellation:

```cpp
services().provide("economy:balance", [](const std::string &request) {
    return std::string("{\"balance\":250}");
});

std::optional<std::string> balance = services().call("economy:balance", "{\"player\":\"Steve\"}");

services().on("quests:completed", [](falcon::CustomEvent &event) {
    event.setData(event.data() + " rewarded");
});

falcon::CustomEventResult result = services().fire("quests:completed", "{\"quest\":\"miner\"}", true);
```

A service belongs to the plugin that provided it and disappears when that plugin is disabled. Call services and
fire custom events from the main thread.

### C# plugins

A C# plugin is a `net8.0` class library that references `Falcon.PluginAPI` and sets `EnableDynamicLoading`:

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net8.0</TargetFramework>
    <EnableDynamicLoading>true</EnableDynamicLoading>
  </PropertyGroup>

  <ItemGroup>
    <PackageReference Include="Falcon.PluginAPI" Version="1.2.0" />
  </ItemGroup>
</Project>
```

```csharp
using Falcon;

namespace MyPlugin
{
    public sealed class Main : Plugin
    {
        public override bool OnEnable()
        {
            Events.On<PlayerJoinEvent>(joinEvent =>
            {
                joinEvent.Player.SendMessage("Welcome!");
            });
            return true;
        }
    }
}
```

Copy the output of `dotnet publish` to `plugins/<name>/`. The manifest uses the `dotnet` runtime, `main` is the
full class name and `assembly` defaults to `<name>.dll`:

```json
{
  "name": "MyPlugin",
  "version": "1.0.0",
  "main": "MyPlugin.Main",
  "runtime": "dotnet",
  "assembly": "MyPlugin.dll"
}
```

The server needs the .NET 8 runtime or newer. It looks for it in `DOTNET_ROOT`, then in the default install
folders. All C# plugins share one runtime and one `Falcon.PluginAPI`, which the server loads from
`plugins/.dotnet/` (`Falcon.PluginAPI.dll` and `Falcon.PluginAPI.runtimeconfig.json`), or from the folder of
the first C# plugin when `plugins/.dotnet/` does not exist. Keep that copy at least as new as the one the
plugins were built against. Each plugin gets its own `AssemblyLoadContext`, so plugins can ship different
versions of the same library.

A plugin can reference another plugin's assembly at compile time and call its classes directly, with the same
types on both sides, as long as it lists that plugin in `depend` or `softdepend`: its assemblies and the
libraries they loaded then come from that plugin instead of a second copy.

### Java plugins

A Java plugin is a Java 22 jar that depends on `falcon-plugin-api` with the `provided` scope:

```xml
<dependency>
    <groupId>io.github.falcon-mc</groupId>
    <artifactId>falcon-plugin-api</artifactId>
    <version>1.2.0</version>
    <scope>provided</scope>
</dependency>
```

```java
package myplugin;

import falcon.api.Plugin;
import falcon.api.event.PlayerJoinEvent;

public final class MyPlugin extends Plugin {
    @Override
    public boolean onEnable() {
        events().on(PlayerJoinEvent.class, event -> {
            event.player().sendMessage("Welcome!");
        });
        return true;
    }
}
```

Put the jar next to a `plugin.json` in `plugins/<name>/`. The manifest uses the `java` runtime, `main` is the
full class name and `jar` defaults to `<name>.jar`:

```json
{
  "name": "MyPlugin",
  "version": "1.0.0",
  "api-version": "1.2",
  "main": "myplugin.MyPlugin",
  "runtime": "java",
  "jar": "MyPlugin.jar"
}
```

The server needs Java 22 or newer. It looks for it in `FALCON_JAVA_HOME`, then in `JAVA_HOME`, and passes the
options in `FALCON_JAVA_OPTIONS` (for example `-Xmx1G`) to the virtual machine. All Java plugins share one
virtual machine and one `falcon-plugin-api-<version>.jar`, which the server loads from `plugins/.java/`: keep it
at least as new as the one the plugins were built against. Each plugin gets its own class loader, and objects
that own native memory, such as a created `Item`, are `AutoCloseable`.

A plugin can compile against another plugin's jar (`provided` scope) and call its classes directly, with the
same classes on both sides, as long as it lists that plugin in `depend` or `softdepend`: its class loader then
looks in that plugin's jar before its own, so a class is never loaded twice.

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

The C# package and its example need the .NET 8 SDK. The interop is generated from `falcon_api.h` on every
build, and the package version follows the API version of that header:

```
dotnet build examples/dotnet/HelloPlugin/HelloPlugin.csproj -c Release
dotnet pack dotnet/Falcon.PluginAPI/Falcon.PluginAPI.csproj -c Release -o nupkg
```

The Java artifact and its example need JDK 22 and Maven. The bindings are generated from `falcon_api.h` on every
build, and the build fails when the artifact version does not match the API version of that header:

```
mvn -B -f java/pom.xml install
mvn -B -f examples/java/HelloPlugin/pom.xml package
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
