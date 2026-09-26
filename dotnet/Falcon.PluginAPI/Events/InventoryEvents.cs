using Falcon.Interop;

namespace Falcon
{
    public sealed class InventoryOpenEvent : Event, IEvent<InventoryOpenEvent>
    {
        private InventoryOpenEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventInventoryOpen;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
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

        public static InventoryOpenEvent Create(nint handle)
        {
            return new InventoryOpenEvent(handle);
        }
    }

    public sealed class InventoryCloseEvent : Event, IEvent<InventoryCloseEvent>
    {
        private InventoryCloseEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventInventoryClose;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
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

        public static InventoryCloseEvent Create(nint handle)
        {
            return new InventoryCloseEvent(handle);
        }
    }

    public sealed unsafe class InventoryTransactionEvent : Event, IEvent<InventoryTransactionEvent>
    {
        private InventoryTransactionEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventInventoryTransaction;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public string Action
        {
            get
            {
                return EventCause();
            }
        }

        public Item Item
        {
            get
            {
                return EventItem();
            }
        }

        public uint Count
        {
            get
            {
                return (uint)EventAmount();
            }
        }

        public string SourceContainer
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventSourceContainer(Raw));
            }
        }

        public int SourceSlot
        {
            get
            {
                return NativeApi.Table->eventSourceSlot(Raw);
            }
        }

        public string DestinationContainer
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventDestinationContainer(Raw));
            }
        }

        public int DestinationSlot
        {
            get
            {
                return NativeApi.Table->eventDestinationSlot(Raw);
            }
        }

        public static InventoryTransactionEvent Create(nint handle)
        {
            return new InventoryTransactionEvent(handle);
        }
    }

    public sealed class CraftItemEvent : Event, IEvent<CraftItemEvent>
    {
        private CraftItemEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventCraftItem;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Item Result
        {
            get
            {
                return EventItem();
            }
        }

        public uint Times
        {
            get
            {
                return (uint)EventAmount();
            }
        }

        public static CraftItemEvent Create(nint handle)
        {
            return new CraftItemEvent(handle);
        }
    }

    public sealed class FurnaceSmeltEvent : Event, IEvent<FurnaceSmeltEvent>
    {
        private FurnaceSmeltEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventFurnaceSmelt;
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

        public Item Source
        {
            get
            {
                return EventItem();
            }
        }

        public Item Result
        {
            get
            {
                return EventResult();
            }
            set
            {
                SetEventResult(value);
            }
        }

        public static FurnaceSmeltEvent Create(nint handle)
        {
            return new FurnaceSmeltEvent(handle);
        }
    }

    public sealed class FurnaceBurnEvent : Event, IEvent<FurnaceBurnEvent>
    {
        private FurnaceBurnEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventFurnaceBurn;
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

        public Item Fuel
        {
            get
            {
                return EventItem();
            }
        }

        public int BurnTime
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

        public static FurnaceBurnEvent Create(nint handle)
        {
            return new FurnaceBurnEvent(handle);
        }
    }

    public sealed class ItemEnchantEvent : Event, IEvent<ItemEnchantEvent>
    {
        private ItemEnchantEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventItemEnchant;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Item Item
        {
            get
            {
                return EventItem();
            }
        }

        public int LevelCost
        {
            get
            {
                return (int)EventAmount();
            }
        }

        public static ItemEnchantEvent Create(nint handle)
        {
            return new ItemEnchantEvent(handle);
        }
    }

    public sealed class ItemDamageEvent : Event, IEvent<ItemDamageEvent>
    {
        private ItemDamageEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventItemDamage;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Item Item
        {
            get
            {
                return EventItem();
            }
        }

        public int Damage
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

        public static ItemDamageEvent Create(nint handle)
        {
            return new ItemDamageEvent(handle);
        }
    }

    public sealed class ItemBreakEvent : Event, IEvent<ItemBreakEvent>
    {
        private ItemBreakEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventItemBreak;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Item Item
        {
            get
            {
                return EventItem();
            }
        }

        public static ItemBreakEvent Create(nint handle)
        {
            return new ItemBreakEvent(handle);
        }
    }
}
