using System;
using System.Collections.Generic;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Level : IEquatable<Level>
    {
        private readonly FalconLevel* _handle;

        internal Level(FalconLevel* handle)
        {
            _handle = handle;
        }

        public nint Handle
        {
            get
            {
                return (nint)_handle;
            }
        }

        public bool IsValid
        {
            get
            {
                return _handle != null;
            }
        }

        public Dimension Dimension
        {
            get
            {
                return (Dimension)NativeApi.Table->levelDimension(_handle);
            }
        }

        public string Name
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->levelName(_handle));
            }
        }

        public long Time
        {
            get
            {
                return NativeApi.Table->levelTime(_handle);
            }
            set
            {
                NativeApi.Table->levelSetTime(_handle, value);
            }
        }

        public bool IsRaining
        {
            get
            {
                return NativeApi.Table->levelIsRaining(_handle) != 0;
            }
            set
            {
                NativeApi.Table->levelSetRaining(_handle, value ? 1 : 0);
            }
        }

        public bool IsThundering
        {
            get
            {
                return NativeApi.Table->levelIsThundering(_handle) != 0;
            }
            set
            {
                NativeApi.Table->levelSetThundering(_handle, value ? 1 : 0);
            }
        }

        public Vec3 SpawnPosition
        {
            get
            {
                return Vec3.From(NativeApi.Table->levelSpawnPosition(_handle));
            }
        }

        public IReadOnlyList<Entity> Entities
        {
            get
            {
                uint count = NativeApi.Table->levelEntityCount(_handle);
                List<Entity> entities = new List<Entity>((int)count);
                for (uint index = 0; index < count; index++)
                {
                    entities.Add(new Entity(NativeApi.Table->levelEntity(_handle, index)));
                }

                return entities;
            }
        }

        internal FalconLevel* Raw
        {
            get
            {
                return _handle;
            }
        }

        public static bool operator ==(Level? left, Level? right)
        {
            if (left is null)
            {
                return right is null;
            }

            return left.Equals(right);
        }

        public static bool operator !=(Level? left, Level? right)
        {
            return !(left == right);
        }

        public Block GetBlock(BlockPos position)
        {
            return new Block(position, BlockName(position), BlockStates(position));
        }

        public string BlockName(BlockPos position)
        {
            return NativeApi.Text(NativeApi.Table->levelGetBlock(_handle, position.ToNative()));
        }

        public string BlockStates(BlockPos position)
        {
            return NativeApi.Text(NativeApi.Table->levelGetBlockStates(_handle, position.ToNative()));
        }

        public bool SetBlock(BlockPos position, string name, string statesJson = "")
        {
            fixed (byte* nameText = NativeApi.Utf8(name))
            fixed (byte* statesText = NativeApi.Nullable(statesJson))
            {
                return NativeApi.Table->levelSetBlock(_handle, position.ToNative(), nameText, statesText) != 0;
            }
        }

        public bool BreakBlock(BlockPos position, bool dropItems = true)
        {
            return NativeApi.Table->levelBreakBlock(_handle, position.ToNative(), dropItems ? 1 : 0) != 0;
        }

        public bool IsChunkLoaded(int chunkX, int chunkZ)
        {
            return NativeApi.Table->levelIsChunkLoaded(_handle, chunkX, chunkZ) != 0;
        }

        public int HighestBlockY(int x, int z)
        {
            return NativeApi.Table->levelHighestBlockY(_handle, x, z);
        }

        public Entity? SpawnEntity(string identifier, Vec3 position)
        {
            fixed (byte* text = NativeApi.Utf8(identifier))
            {
                return Entity.Wrap(NativeApi.Table->levelSpawnEntity(_handle, text, position.ToNative()));
            }
        }

        public void DropItem(Vec3 position, Item item)
        {
            NativeApi.Table->levelDropItem(_handle, position.ToNative(), item.Raw);
        }

        public void StrikeLightning(Vec3 position)
        {
            NativeApi.Table->levelStrikeLightning(_handle, position.ToNative());
        }

        public void CreateExplosion(Vec3 position, float power, bool breakBlocks = true)
        {
            NativeApi.Table->levelCreateExplosion(_handle, position.ToNative(), power, breakBlocks ? 1 : 0);
        }

        public bool Equals(Level? other)
        {
            return other is not null && other._handle == _handle;
        }

        public override bool Equals(object? other)
        {
            return Equals(other as Level);
        }

        public override int GetHashCode()
        {
            return ((nint)_handle).GetHashCode();
        }

        internal static Level? Wrap(FalconLevel* handle)
        {
            if (handle == null)
            {
                return null;
            }

            return new Level(handle);
        }
    }
}
