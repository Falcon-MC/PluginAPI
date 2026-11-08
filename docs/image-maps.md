# Image maps

Image maps put any picture on vanilla maps hung in item frames. Players need no resource pack or mod: the client
draws the pixels the server sends.

Each map is 128 x 128 pixels. A larger image is cut into a grid of maps, row by row from the top left, and the grid
is hung on a wall of item frames. The maps are ordinary locked maps, saved with the world, so they survive a
restart and can be copied on a cartography table.

## What the client does with the pixels

Checked in the 26.60 client:

- A map pixel is any 32-bit RGBA value. The client copies the colours of `ClientboundMapItemDataPacket` straight
  into the map, without matching them to the block colour palette.
- The map texture is `R8G8B8A8`. Alpha reaches the texture unchanged.
- A pixel of exactly `0` (transparent black) is drawn as the faint checker the client uses for unexplored areas.
  Use opaque pixels when the image should look the same everywhere.
- A locked map is never redrawn by the client, so the server must lock image maps. Falcon always does.

## Limits

| What | Limit |
|---|---|
| Map size | 128 x 128 pixels |
| Grid | 32 x 32 maps |
| Source image | 4096 pixels a side |
| PNG file | 32 MB |

Pixels are RGBA bytes, four per pixel, rows from top to bottom. PNG files of any colour type and bit depth are
accepted and converted to 8-bit RGBA. A call that fails changes nothing and `lastError` gives the reason.

## Grid size and fit

`columns` and `rows` choose the grid. When both are `0`, the grid has one map for every started 128 pixels of the
image. When one of them is `0`, it follows the aspect ratio of the image.

When the image and the grid have different aspect ratios, `fit` decides:

| Fit | Result |
|---|---|
| `CONTAIN` | the whole image, centred, with transparent margins |
| `COVER` | the whole grid covered, centred, with the overflow cropped |
| `STRETCH` | the image scaled to the grid and distorted |

Downscaling averages the covered pixels, and upscaling repeats them, so pixel art stays sharp.

## Hanging a grid

`place` hangs the grid in item frames. `topLeft` is the frame of the first map, and `facing` is the way the frames
look out of the wall: a wall of frames facing `SOUTH` is read by a player standing south of it and looking north.

| Facing | Columns go toward |
|---|---|
| `NORTH` | west |
| `SOUTH` | east |
| `WEST` | south |
| `EAST` | north |

Rows go down. Each spot must be air with a solid block behind it, or an item frame with the same facing that is
empty or already holds an image map. Every spot is checked before anything changes. With `glowing`, the frames are
glow item frames, which keep the picture lit in the dark.

## Keeping and removing maps

Every create call makes new maps with new ids, and they stay in the world save until they are removed. A plugin
that makes its pictures in `onEnable` adds a grid to the world at every restart unless it reuses them: save the ids
the first time, in the plugin config for example, and on the next start check them with `isImageMap` before making
new ones. `update` redraws a kept map in place.

`remove` erases an image map from memory and from the world save. It also empties the item frames of loaded chunks
that hold it and takes it out of the inventories and off hands of online players. Maps it cannot reach stay where
they are: frames in unloaded chunks, chests and other containers, dropped items and the inventories of offline
players. Such a map keeps its item but has no data any more: the server never answers for its id, so the client
shows it blank, and it is never redrawn by the world or turned into a new map. A client that already received the
picture may keep showing it until it reconnects.

## Live updates

`update` replaces a block of pixels in an image map, for animations or live displays. Only the pixels that really
changed are sent, and only to the players whose client asked for that map, framed or held.

## C++

```cpp
#include <falcon/Falcon.hpp>

#include <fstream>
#include <iterator>

std::ifstream file(dataFolder() + "/logo.png", std::ios::binary);
const std::vector<uint8_t> png((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

falcon::Level level = player.entity().level();
falcon::ImageMapGrid grid = falcon::ImageMaps::createFromPng(level, png, 4, 0, falcon::ImageFit::Contain);
if (grid.empty()) {
    logger().warning("logo: " + falcon::ImageMaps::lastError());
    return;
}

falcon::ImageMaps::place(level, falcon::BlockPos(10, 72, -4), falcon::WallFacing::South, grid);
player.giveMap(grid.ids[0]);

std::vector<uint8_t> red(16 * 16 * 4, 0xff);
for (size_t index = 0; index < red.size(); index += 4)
    red[index + 1] = red[index + 2] = 0;
falcon::ImageMaps::update(level, grid.ids[0], 56, 56, 16, 16, red);
```

| Function | Returns |
|---|---|
| `ImageMaps::create(level, rgba)` | one map from 128 * 128 * 4 bytes, `-1` on failure |
| `ImageMaps::create(level, rgba, width, height, columns, rows, fit)` | `ImageMapGrid` |
| `ImageMaps::createFromPng(level, png, columns, rows, fit)` | `ImageMapGrid` |
| `ImageMaps::place(level, topLeft, facing, grid, glowing)` | `bool` |
| `ImageMaps::update(level, mapId, x, y, width, height, rgba)` | `bool` |
| `ImageMaps::isImageMap(level, mapId)` | `bool` |
| `ImageMaps::remove(level, mapId)` | `bool`; `false` when the id is not an image map |
| `ImageMaps::lastError()` | the reason of the last failure |
| `Player::giveMap(mapId)` | `bool`; the map drops at the player's feet when the inventory is full |

## C#

```csharp
byte[] png = File.ReadAllBytes(Path.Combine(DataFolder, "logo.png"));
Level level = player.Entity.Level;

ImageMapGrid grid = ImageMaps.CreateFromPng(level, png, columns: 4, fit: ImageFit.Contain);
if (grid.IsEmpty)
{
    Logger.Warning("logo: " + ImageMaps.LastError);
    return;
}

ImageMaps.Place(level, new BlockPos(10, 72, -4), WallFacing.South, grid, glowing: true);
player.GiveMap(grid.Ids[0]);
```

`ImageMaps.Remove(level, mapId)` erases a map, as described above.

## Java

```java
byte[] png = Files.readAllBytes(dataFolder().resolve("logo.png"));
Level level = player.entity().level();

ImageMapGrid grid = ImageMaps.createFromPng(level, png, 4, 0, ImageFit.CONTAIN);
if (grid.isEmpty()) {
    logger().warning("logo: " + ImageMaps.lastError());
    return;
}

ImageMaps.place(level, new BlockPos(10, 72, -4), WallFacing.SOUTH, grid, false);
player.giveMap(grid.ids()[0]);
```

`ImageMaps.remove(level, mapId)` erases a map, as described above.

## C API

```c
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
```

The grid functions return the number of maps they made, `0` on failure. `outColumns` and `outRows` receive the grid
size even when `capacity` is too small, so a caller can ask once with a capacity of `0` and allocate. Maps are made
only when every id fits. Maps belong to the world of `level`, whatever its dimension. `mapPlaceImageGrid` reads
`count` ids and refuses any count other than `columns * rows`.
