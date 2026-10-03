using Falcon.Interop;

namespace Falcon
{
    public sealed class PlayerJoinEvent : Event, IEvent<PlayerJoinEvent>
    {
        private PlayerJoinEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerJoin;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public static PlayerJoinEvent Create(nint handle)
        {
            return new PlayerJoinEvent(handle);
        }
    }

    public sealed class PlayerQuitEvent : Event, IEvent<PlayerQuitEvent>
    {
        private PlayerQuitEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerQuit;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public static PlayerQuitEvent Create(nint handle)
        {
            return new PlayerQuitEvent(handle);
        }
    }

    public sealed class PlayerChatEvent : Event, IEvent<PlayerChatEvent>
    {
        private PlayerChatEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerChat;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public string Message
        {
            get
            {
                return EventMessage();
            }
            set
            {
                SetEventMessage(value);
            }
        }

        public static PlayerChatEvent Create(nint handle)
        {
            return new PlayerChatEvent(handle);
        }
    }

    public sealed class PlayerDeathEvent : Event, IEvent<PlayerDeathEvent>
    {
        private PlayerDeathEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerDeath;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public string Message
        {
            get
            {
                return EventMessage();
            }
            set
            {
                SetEventMessage(value);
            }
        }

        public static PlayerDeathEvent Create(nint handle)
        {
            return new PlayerDeathEvent(handle);
        }
    }

    public sealed class PlayerRespawnEvent : Event, IEvent<PlayerRespawnEvent>
    {
        private PlayerRespawnEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerRespawn;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Vec3 Position
        {
            get
            {
                return EventTo();
            }
            set
            {
                SetEventTo(value);
            }
        }

        public static PlayerRespawnEvent Create(nint handle)
        {
            return new PlayerRespawnEvent(handle);
        }
    }

    public sealed class PlayerMoveEvent : Event, IEvent<PlayerMoveEvent>
    {
        private PlayerMoveEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerMove;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Vec3 From
        {
            get
            {
                return EventFrom();
            }
        }

        public Vec3 To
        {
            get
            {
                return EventTo();
            }
            set
            {
                SetEventTo(value);
            }
        }

        public static PlayerMoveEvent Create(nint handle)
        {
            return new PlayerMoveEvent(handle);
        }
    }

    public sealed class PlayerDropItemEvent : Event, IEvent<PlayerDropItemEvent>
    {
        private PlayerDropItemEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerDropItem;
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

        public static PlayerDropItemEvent Create(nint handle)
        {
            return new PlayerDropItemEvent(handle);
        }
    }

    public sealed class PlayerPickupItemEvent : Event, IEvent<PlayerPickupItemEvent>
    {
        private PlayerPickupItemEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerPickupItem;
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

        public static PlayerPickupItemEvent Create(nint handle)
        {
            return new PlayerPickupItemEvent(handle);
        }
    }

    public sealed class PlayerCommandEvent : Event, IEvent<PlayerCommandEvent>
    {
        private PlayerCommandEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerCommand;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public string Command
        {
            get
            {
                return EventMessage();
            }
            set
            {
                SetEventMessage(value);
            }
        }

        public static PlayerCommandEvent Create(nint handle)
        {
            return new PlayerCommandEvent(handle);
        }
    }

    public sealed unsafe class PlayerGameModeChangeEvent : Event, IEvent<PlayerGameModeChangeEvent>
    {
        private PlayerGameModeChangeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerGameModeChange;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public GameMode GameMode
        {
            get
            {
                return (GameMode)NativeApi.Table->eventGameMode(Raw);
            }
        }

        public GameMode PreviousGameMode
        {
            get
            {
                return (GameMode)NativeApi.Table->eventPreviousGameMode(Raw);
            }
        }

        public static PlayerGameModeChangeEvent Create(nint handle)
        {
            return new PlayerGameModeChangeEvent(handle);
        }
    }

    public sealed unsafe class PlayerChangeDimensionEvent : Event, IEvent<PlayerChangeDimensionEvent>
    {
        private PlayerChangeDimensionEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerChangeDimension;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Dimension Dimension
        {
            get
            {
                return (Dimension)NativeApi.Table->eventDimension(Raw);
            }
        }

        public Dimension PreviousDimension
        {
            get
            {
                return (Dimension)NativeApi.Table->eventPreviousDimension(Raw);
            }
        }

        public Vec3 From
        {
            get
            {
                return EventFrom();
            }
        }

        public Vec3 To
        {
            get
            {
                return EventTo();
            }
            set
            {
                SetEventTo(value);
            }
        }

        public static PlayerChangeDimensionEvent Create(nint handle)
        {
            return new PlayerChangeDimensionEvent(handle);
        }
    }

    public sealed unsafe class PlayerChangeWorldEvent : Event, IEvent<PlayerChangeWorldEvent>
    {
        private PlayerChangeWorldEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerChangeWorld;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public string World
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventWorldName(Raw));
            }
        }

        public string PreviousWorld
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventPreviousWorldName(Raw));
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public Vec3 From
        {
            get
            {
                return EventFrom();
            }
        }

        public Vec3 To
        {
            get
            {
                return EventTo();
            }
            set
            {
                SetEventTo(value);
            }
        }

        public static PlayerChangeWorldEvent Create(nint handle)
        {
            return new PlayerChangeWorldEvent(handle);
        }
    }

    public sealed class PlayerInteractEntityEvent : Event, IEvent<PlayerInteractEntityEvent>
    {
        private PlayerInteractEntityEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerInteractEntity;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public Entity Entity
        {
            get
            {
                return EventEntity();
            }
        }

        public static PlayerInteractEntityEvent Create(nint handle)
        {
            return new PlayerInteractEntityEvent(handle);
        }
    }

    public sealed unsafe class PlayerPreLoginEvent : Event, IEvent<PlayerPreLoginEvent>
    {
        private PlayerPreLoginEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerPreLogin;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public string KickMessage
        {
            get
            {
                return EventMessage();
            }
            set
            {
                SetEventMessage(value);
            }
        }

        /// <summary>
        /// The client data sent in the login request, as the JSON object the client signed.
        /// </summary>
        public string ClientData
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventClientData(Raw));
            }
        }

        public string ServerAddress
        {
            get
            {
                return ClientDataField("ServerAddress");
            }
        }

        public string GameVersion
        {
            get
            {
                return ClientDataField("GameVersion");
            }
        }

        public string DeviceModel
        {
            get
            {
                return ClientDataField("DeviceModel");
            }
        }

        public string DeviceOS
        {
            get
            {
                return ClientDataField("DeviceOS");
            }
        }

        public string ThirdPartyName
        {
            get
            {
                return ClientDataField("ThirdPartyName");
            }
        }

        /// <summary>
        /// One top-level field of the client data, as text; empty when the client did not send it.
        /// </summary>
        public string ClientDataField(string key)
        {
            fixed (byte* text = NativeApi.Utf8(key))
            {
                return NativeApi.Text(NativeApi.Table->eventClientDataField(Raw, text));
            }
        }

        public static PlayerPreLoginEvent Create(nint handle)
        {
            return new PlayerPreLoginEvent(handle);
        }
    }

    /// <summary>
    /// Fired when a client opens a connection, before it sends any game packet. Cancelling it closes the
    /// connection without a disconnect screen.
    /// </summary>
    /// <summary>
    /// Fired when a player answers or closes a form sent with Player.SendForm.
    /// </summary>
    public sealed unsafe class PlayerFormResponseEvent : Event, IEvent<PlayerFormResponseEvent>
    {
        private PlayerFormResponseEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerFormResponse;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public uint FormId
        {
            get
            {
                return NativeApi.Table->eventFormId(Raw);
            }
        }

        /// <summary>
        /// The answer as JSON: the button index, a boolean or the array of custom form values. Empty when the
        /// player closed the form.
        /// </summary>
        public string Response
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventFormResponse(Raw));
            }
        }

        public bool Closed
        {
            get
            {
                return NativeApi.Table->eventState(Raw) != 0;
            }
        }

        public static PlayerFormResponseEvent Create(nint handle)
        {
            return new PlayerFormResponseEvent(handle);
        }
    }

    /// <summary>
    /// Fired when a client opens a connection, before it sends any game packet. Cancelling it closes the
    /// connection without a disconnect screen.
    /// </summary>
    public sealed unsafe class ConnectionOpenEvent : Event, IEvent<ConnectionOpenEvent>
    {
        private ConnectionOpenEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventConnectionOpen;
            }
        }

        public string Address
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventAddress(Raw));
            }
        }

        /// <summary>
        /// The transport the client connected with: "raknet" or "nethernet".
        /// </summary>
        public string Transport
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventTransport(Raw));
            }
        }

        /// <summary>
        /// The GUID the client announced during the RakNet handshake, or 0 on other transports.
        /// </summary>
        public ulong ClientGuid
        {
            get
            {
                return NativeApi.Table->eventClientGuid(Raw);
            }
        }

        /// <summary>
        /// The datagram size negotiated during the RakNet handshake, or 0 on other transports.
        /// </summary>
        public uint MtuSize
        {
            get
            {
                return NativeApi.Table->eventMtuSize(Raw);
            }
        }

        public static ConnectionOpenEvent Create(nint handle)
        {
            return new ConnectionOpenEvent(handle);
        }
    }

    public sealed class PlayerToggleSneakEvent : Event, IEvent<PlayerToggleSneakEvent>
    {
        private PlayerToggleSneakEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerToggleSneak;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public bool IsSneaking
        {
            get
            {
                return EventState();
            }
        }

        public static PlayerToggleSneakEvent Create(nint handle)
        {
            return new PlayerToggleSneakEvent(handle);
        }
    }

    public sealed class PlayerToggleSprintEvent : Event, IEvent<PlayerToggleSprintEvent>
    {
        private PlayerToggleSprintEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerToggleSprint;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public bool IsSprinting
        {
            get
            {
                return EventState();
            }
        }

        public static PlayerToggleSprintEvent Create(nint handle)
        {
            return new PlayerToggleSprintEvent(handle);
        }
    }

    public sealed class PlayerToggleFlightEvent : Event, IEvent<PlayerToggleFlightEvent>
    {
        private PlayerToggleFlightEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerToggleFlight;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public bool IsFlying
        {
            get
            {
                return EventState();
            }
        }

        public static PlayerToggleFlightEvent Create(nint handle)
        {
            return new PlayerToggleFlightEvent(handle);
        }
    }

    public sealed class PlayerItemHeldEvent : Event, IEvent<PlayerItemHeldEvent>
    {
        private PlayerItemHeldEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerItemHeld;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public int PreviousSlot
        {
            get
            {
                return EventSourceSlot();
            }
        }

        public int NewSlot
        {
            get
            {
                return EventDestinationSlot();
            }
        }

        public Item Item
        {
            get
            {
                return EventItem();
            }
        }

        public static PlayerItemHeldEvent Create(nint handle)
        {
            return new PlayerItemHeldEvent(handle);
        }
    }

    public sealed class PlayerItemConsumeEvent : Event, IEvent<PlayerItemConsumeEvent>
    {
        private PlayerItemConsumeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerItemConsume;
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

        public static PlayerItemConsumeEvent Create(nint handle)
        {
            return new PlayerItemConsumeEvent(handle);
        }
    }

    public sealed class PlayerBedEnterEvent : Event, IEvent<PlayerBedEnterEvent>
    {
        private PlayerBedEnterEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerBedEnter;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public BlockPos Bed
        {
            get
            {
                return EventBlockPosition();
            }
        }

        public static PlayerBedEnterEvent Create(nint handle)
        {
            return new PlayerBedEnterEvent(handle);
        }
    }

    public sealed class PlayerBedLeaveEvent : Event, IEvent<PlayerBedLeaveEvent>
    {
        private PlayerBedLeaveEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerBedLeave;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public BlockPos Bed
        {
            get
            {
                return EventBlockPosition();
            }
        }

        public static PlayerBedLeaveEvent Create(nint handle)
        {
            return new PlayerBedLeaveEvent(handle);
        }
    }

    public sealed class PlayerJumpEvent : Event, IEvent<PlayerJumpEvent>
    {
        private PlayerJumpEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerJump;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public static PlayerJumpEvent Create(nint handle)
        {
            return new PlayerJumpEvent(handle);
        }
    }

    public sealed class PlayerFoodChangeEvent : Event, IEvent<PlayerFoodChangeEvent>
    {
        private PlayerFoodChangeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerFoodChange;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public double Food
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

        public double PreviousFood
        {
            get
            {
                return EventPreviousAmount();
            }
        }

        public static PlayerFoodChangeEvent Create(nint handle)
        {
            return new PlayerFoodChangeEvent(handle);
        }
    }

    public sealed class PlayerExperienceChangeEvent : Event, IEvent<PlayerExperienceChangeEvent>
    {
        private PlayerExperienceChangeEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventPlayerExperienceChange;
            }
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public int Amount
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

        public static PlayerExperienceChangeEvent Create(nint handle)
        {
            return new PlayerExperienceChangeEvent(handle);
        }
    }
}
