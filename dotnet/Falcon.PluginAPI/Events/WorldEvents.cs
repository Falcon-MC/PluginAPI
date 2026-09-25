using System.Collections.Generic;
using Falcon.Interop;

namespace Falcon
{
    public sealed class ProjectileHitEvent : Event, IEvent<ProjectileHitEvent>
    {
        private ProjectileHitEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventProjectileHit;
            }
        }

        public Entity Projectile
        {
            get
            {
                return EventEntity();
            }
        }

        public Entity? Shooter
        {
            get
            {
                return EventAttacker();
            }
        }

        public Entity? Target
        {
            get
            {
                return EventTarget();
            }
        }

        public bool HitBlock
        {
            get
            {
                return EventBlockName().Length > 0;
            }
        }

        public BlockPos BlockPosition
        {
            get
            {
                return EventBlockPosition();
            }
        }

        public string BlockName
        {
            get
            {
                return EventBlockName();
            }
        }

        public Vec3 Position
        {
            get
            {
                return EventPosition();
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public static ProjectileHitEvent Create(nint handle)
        {
            return new ProjectileHitEvent(handle);
        }
    }

    public sealed unsafe class ExplosionEvent : Event, IEvent<ExplosionEvent>
    {
        private ExplosionEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventExplosion;
            }
        }

        public Entity? Source
        {
            get
            {
                return OptionalEntity();
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public Vec3 Position
        {
            get
            {
                return EventPosition();
            }
        }

        public double Power
        {
            get
            {
                return EventAmount();
            }
            set
            {
                SetEventAmount(value);
            }
        }

        public IReadOnlyList<BlockPos> Blocks
        {
            get
            {
                uint count = NativeApi.Table->eventBlockCount(Raw);
                List<BlockPos> blocks = new List<BlockPos>((int)count);
                for (uint index = 0; index < count; index++)
                {
                    blocks.Add(BlockPos.From(NativeApi.Table->eventBlockAt(Raw, index)));
                }

                return blocks;
            }
            set
            {
                FalconBlockPos[] positions = new FalconBlockPos[value.Count];
                for (int index = 0; index < positions.Length; index++)
                {
                    positions[index] = value[index].ToNative();
                }

                fixed (FalconBlockPos* raw = positions)
                {
                    NativeApi.Table->eventSetBlocks(Raw, raw, (uint)positions.Length);
                }
            }
        }

        public static ExplosionEvent Create(nint handle)
        {
            return new ExplosionEvent(handle);
        }
    }

    public sealed class FireSpreadEvent : Event, IEvent<FireSpreadEvent>
    {
        private FireSpreadEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventFireSpread;
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public BlockPos Position
        {
            get
            {
                return EventBlockPosition();
            }
        }

        public Vec3 Source
        {
            get
            {
                return EventPosition();
            }
        }

        public static FireSpreadEvent Create(nint handle)
        {
            return new FireSpreadEvent(handle);
        }
    }

    public sealed class BlockBurnEvent : Event, IEvent<BlockBurnEvent>
    {
        private BlockBurnEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockBurn;
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public BlockPos Position
        {
            get
            {
                return EventBlockPosition();
            }
        }

        public string BlockName
        {
            get
            {
                return EventBlockName();
            }
        }

        public Vec3 Source
        {
            get
            {
                return EventPosition();
            }
        }

        public static BlockBurnEvent Create(nint handle)
        {
            return new BlockBurnEvent(handle);
        }
    }
}
