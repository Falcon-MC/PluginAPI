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

    public sealed class ProjectileLaunchEvent : Event, IEvent<ProjectileLaunchEvent>
    {
        private ProjectileLaunchEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventProjectileLaunch;
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

        public static ProjectileLaunchEvent Create(nint handle)
        {
            return new ProjectileLaunchEvent(handle);
        }
    }

    public abstract class BlockChangeEvent : Event
    {
        private protected BlockChangeEvent(nint handle) : base(handle)
        {
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

        public string PreviousBlockName
        {
            get
            {
                return EventPreviousBlockName();
            }
        }
    }

    public sealed class BlockGrowEvent : BlockChangeEvent, IEvent<BlockGrowEvent>
    {
        private BlockGrowEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockGrow;
            }
        }

        public static BlockGrowEvent Create(nint handle)
        {
            return new BlockGrowEvent(handle);
        }
    }

    public sealed class BlockSpreadEvent : BlockChangeEvent, IEvent<BlockSpreadEvent>
    {
        private BlockSpreadEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockSpread;
            }
        }

        public Vec3 Source
        {
            get
            {
                return EventFrom();
            }
        }

        public static BlockSpreadEvent Create(nint handle)
        {
            return new BlockSpreadEvent(handle);
        }
    }

    public sealed class BlockFormEvent : BlockChangeEvent, IEvent<BlockFormEvent>
    {
        private BlockFormEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockForm;
            }
        }

        public static BlockFormEvent Create(nint handle)
        {
            return new BlockFormEvent(handle);
        }
    }

    public sealed class BlockFadeEvent : BlockChangeEvent, IEvent<BlockFadeEvent>
    {
        private BlockFadeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockFade;
            }
        }

        public static BlockFadeEvent Create(nint handle)
        {
            return new BlockFadeEvent(handle);
        }
    }

    public sealed class LeavesDecayEvent : BlockChangeEvent, IEvent<LeavesDecayEvent>
    {
        private LeavesDecayEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventLeavesDecay;
            }
        }

        public static LeavesDecayEvent Create(nint handle)
        {
            return new LeavesDecayEvent(handle);
        }
    }

    public sealed class LiquidFlowEvent : Event, IEvent<LiquidFlowEvent>
    {
        private LiquidFlowEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventLiquidFlow;
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
                return EventFrom();
            }
        }

        public string Liquid
        {
            get
            {
                return EventBlockName();
            }
        }

        public static LiquidFlowEvent Create(nint handle)
        {
            return new LiquidFlowEvent(handle);
        }
    }

    public abstract class PistonEvent : Event
    {
        private protected PistonEvent(nint handle) : base(handle)
        {
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

        public uint Face
        {
            get
            {
                return EventBlockFace();
            }
        }

        public IReadOnlyList<BlockPos> Blocks
        {
            get
            {
                return EventBlocks();
            }
        }
    }

    public sealed class PistonExtendEvent : PistonEvent, IEvent<PistonExtendEvent>
    {
        private PistonExtendEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPistonExtend;
            }
        }

        public static PistonExtendEvent Create(nint handle)
        {
            return new PistonExtendEvent(handle);
        }
    }

    public sealed class PistonRetractEvent : PistonEvent, IEvent<PistonRetractEvent>
    {
        private PistonRetractEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPistonRetract;
            }
        }

        public static PistonRetractEvent Create(nint handle)
        {
            return new PistonRetractEvent(handle);
        }
    }

    public sealed class RedstoneChangeEvent : Event, IEvent<RedstoneChangeEvent>
    {
        private RedstoneChangeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventRedstoneChange;
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

        public int Power
        {
            get
            {
                return (int)EventAmount();
            }
            set
            {
                SetEventAmount(value);
            }
        }

        public int PreviousPower
        {
            get
            {
                return (int)EventPreviousAmount();
            }
        }

        public static RedstoneChangeEvent Create(nint handle)
        {
            return new RedstoneChangeEvent(handle);
        }
    }

    public sealed class WeatherChangeEvent : Event, IEvent<WeatherChangeEvent>
    {
        private WeatherChangeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventWeatherChange;
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public bool IsRaining
        {
            get
            {
                return EventState();
            }
        }

        public static WeatherChangeEvent Create(nint handle)
        {
            return new WeatherChangeEvent(handle);
        }
    }

    public sealed class ThunderChangeEvent : Event, IEvent<ThunderChangeEvent>
    {
        private ThunderChangeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventThunderChange;
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public bool IsThundering
        {
            get
            {
                return EventState();
            }
        }

        public static ThunderChangeEvent Create(nint handle)
        {
            return new ThunderChangeEvent(handle);
        }
    }

    public sealed class ChunkLoadEvent : Event, IEvent<ChunkLoadEvent>
    {
        private ChunkLoadEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventChunkLoad;
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public int ChunkX
        {
            get
            {
                return EventChunkX();
            }
        }

        public int ChunkZ
        {
            get
            {
                return EventChunkZ();
            }
        }

        public bool IsNewChunk
        {
            get
            {
                return EventState();
            }
        }

        public static ChunkLoadEvent Create(nint handle)
        {
            return new ChunkLoadEvent(handle);
        }
    }

    public sealed class ChunkUnloadEvent : Event, IEvent<ChunkUnloadEvent>
    {
        private ChunkUnloadEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventChunkUnload;
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public int ChunkX
        {
            get
            {
                return EventChunkX();
            }
        }

        public int ChunkZ
        {
            get
            {
                return EventChunkZ();
            }
        }

        public static ChunkUnloadEvent Create(nint handle)
        {
            return new ChunkUnloadEvent(handle);
        }
    }
}
