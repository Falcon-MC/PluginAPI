using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class ServerTickEvent : Event, IEvent<ServerTickEvent>
    {
        private ServerTickEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventServerTick;
            }
        }

        public ulong Tick
        {
            get
            {
                return NativeApi.Table->eventTick(Raw);
            }
        }

        public static ServerTickEvent Create(nint handle)
        {
            return new ServerTickEvent(handle);
        }
    }

    public sealed class ServerCommandEvent : Event, IEvent<ServerCommandEvent>
    {
        private ServerCommandEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventServerCommand;
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

        public static ServerCommandEvent Create(nint handle)
        {
            return new ServerCommandEvent(handle);
        }
    }
}
