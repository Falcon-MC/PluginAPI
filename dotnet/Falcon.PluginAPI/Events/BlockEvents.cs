using Falcon.Interop;

namespace Falcon
{
    public sealed class BlockBreakEvent : Event, IEvent<BlockBreakEvent>
    {
        private BlockBreakEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockBreak;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
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

        public static BlockBreakEvent Create(nint handle)
        {
            return new BlockBreakEvent(handle);
        }
    }

    public sealed class BlockPlaceEvent : Event, IEvent<BlockPlaceEvent>
    {
        private BlockPlaceEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventBlockPlace;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
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

        public static BlockPlaceEvent Create(nint handle)
        {
            return new BlockPlaceEvent(handle);
        }
    }

    public sealed unsafe class PlayerInteractBlockEvent : Event, IEvent<PlayerInteractBlockEvent>
    {
        private PlayerInteractBlockEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerInteractBlock;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
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
                return NativeApi.Table->eventBlockFace(Raw);
            }
        }

        public static PlayerInteractBlockEvent Create(nint handle)
        {
            return new PlayerInteractBlockEvent(handle);
        }
    }
}
