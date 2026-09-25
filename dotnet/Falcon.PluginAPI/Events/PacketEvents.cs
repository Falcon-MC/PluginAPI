using System;
using Falcon.Interop;

namespace Falcon
{
    public abstract unsafe class DataPacketEvent : Event
    {
        private protected DataPacketEvent(nint handle) : base(handle)
        {
        }

        public Player Player
        {
            get
            {
                return EventPlayer();
            }
        }

        public uint PacketId
        {
            get
            {
                return NativeApi.Table->eventPacketId(Raw);
            }
        }

        public ReadOnlySpan<byte> DataView
        {
            get
            {
                uint length = 0;
                byte* bytes = NativeApi.Table->eventPacketData(Raw, &length);
                if (bytes == null)
                {
                    return ReadOnlySpan<byte>.Empty;
                }

                return new ReadOnlySpan<byte>(bytes, (int)length);
            }
        }

        public byte[] Data
        {
            get
            {
                return DataView.ToArray();
            }
        }

        public void SetData(ReadOnlySpan<byte> data)
        {
            fixed (byte* bytes = data)
            {
                NativeApi.Table->eventSetPacketData(Raw, bytes, (uint)data.Length);
            }
        }
    }

    public sealed class DataPacketReceiveEvent : DataPacketEvent, IEvent<DataPacketReceiveEvent>
    {
        private DataPacketReceiveEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventDataPacketReceive;
            }
        }

        public static DataPacketReceiveEvent Create(nint handle)
        {
            return new DataPacketReceiveEvent(handle);
        }
    }

    public sealed class DataPacketSendEvent : DataPacketEvent, IEvent<DataPacketSendEvent>
    {
        private DataPacketSendEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventDataPacketSend;
            }
        }

        public static DataPacketSendEvent Create(nint handle)
        {
            return new DataPacketSendEvent(handle);
        }
    }
}
