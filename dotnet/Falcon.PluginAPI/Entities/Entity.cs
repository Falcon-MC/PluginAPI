using System;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Entity : IEquatable<Entity>
    {
        private readonly FalconEntity* _handle;

        internal Entity(FalconEntity* handle)
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

        public string Type
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->entityType(_handle));
            }
        }

        public ulong RuntimeId
        {
            get
            {
                return NativeApi.Table->entityRuntimeId(_handle);
            }
        }

        public Level Level
        {
            get
            {
                return new Level(NativeApi.Table->entityLevel(_handle));
            }
        }

        public Vec3 Position
        {
            get
            {
                return Vec3.From(NativeApi.Table->entityPosition(_handle));
            }
        }

        public Vec3 Rotation
        {
            get
            {
                return Vec3.From(NativeApi.Table->entityRotation(_handle));
            }
        }

        public Vec3 Motion
        {
            get
            {
                return Vec3.From(NativeApi.Table->entityMotion(_handle));
            }
            set
            {
                NativeApi.Table->entitySetMotion(_handle, value.ToNative());
            }
        }

        public float Health
        {
            get
            {
                return NativeApi.Table->entityHealth(_handle);
            }
            set
            {
                NativeApi.Table->entitySetHealth(_handle, value);
            }
        }

        public float MaxHealth
        {
            get
            {
                return NativeApi.Table->entityMaxHealth(_handle);
            }
        }

        public bool IsAlive
        {
            get
            {
                return NativeApi.Table->entityIsAlive(_handle) != 0;
            }
        }

        public string NameTag
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->entityNameTag(_handle));
            }
            set
            {
                fixed (byte* text = NativeApi.Utf8(value))
                {
                    NativeApi.Table->entitySetNameTag(_handle, text);
                }
            }
        }

        public bool IsOnFire
        {
            get
            {
                return NativeApi.Table->entityIsOnFire(_handle) != 0;
            }
        }

        internal FalconEntity* Raw
        {
            get
            {
                return _handle;
            }
        }

        public static bool operator ==(Entity? left, Entity? right)
        {
            if (left is null)
            {
                return right is null;
            }

            return left.Equals(right);
        }

        public static bool operator !=(Entity? left, Entity? right)
        {
            return !(left == right);
        }

        public void Teleport(Vec3 position)
        {
            NativeApi.Table->entityTeleport(_handle, null, position.ToNative());
        }

        public void Teleport(Vec3 position, Level level)
        {
            NativeApi.Table->entityTeleport(_handle, level.Raw, position.ToNative());
        }

        public bool Damage(float amount, string cause = "")
        {
            fixed (byte* text = NativeApi.Nullable(cause))
            {
                return NativeApi.Table->entityDamage(_handle, amount, text, null) != 0;
            }
        }

        public bool Damage(float amount, string cause, Entity attacker)
        {
            fixed (byte* text = NativeApi.Nullable(cause))
            {
                return NativeApi.Table->entityDamage(_handle, amount, text, attacker._handle) != 0;
            }
        }

        public void Kill()
        {
            NativeApi.Table->entityKill(_handle);
        }

        public void Remove()
        {
            NativeApi.Table->entityRemove(_handle);
        }

        public void SetOnFire(uint ticks)
        {
            NativeApi.Table->entitySetOnFire(_handle, ticks);
        }

        public void Extinguish()
        {
            SetOnFire(0);
        }

        public Player? AsPlayer()
        {
            return Player.Wrap(NativeApi.Table->entityPlayer(_handle));
        }

        public bool Equals(Entity? other)
        {
            return other is not null && other._handle == _handle;
        }

        public override bool Equals(object? other)
        {
            return Equals(other as Entity);
        }

        public override int GetHashCode()
        {
            return ((nint)_handle).GetHashCode();
        }

        internal static Entity? Wrap(FalconEntity* handle)
        {
            if (handle == null)
            {
                return null;
            }

            return new Entity(handle);
        }
    }
}
