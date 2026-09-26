using System.Collections.Generic;
using Falcon.Interop;

namespace Falcon
{
    public abstract unsafe class Event
    {
        protected Event(nint handle)
        {
            Handle = handle;
        }

        public nint Handle { get; }

        public bool IsCancellable
        {
            get
            {
                return NativeApi.Table->eventIsCancellable(Raw) != 0;
            }
        }

        public bool IsCancelled
        {
            get
            {
                return NativeApi.Table->eventIsCancelled(Raw) != 0;
            }
            set
            {
                NativeApi.Table->eventSetCancelled(Raw, value ? 1 : 0);
            }
        }

        internal FalconEvent* Raw
        {
            get
            {
                return (FalconEvent*)Handle;
            }
        }

        public void Cancel()
        {
            IsCancelled = true;
        }

        private protected Player EventPlayer()
        {
            return new Player(NativeApi.Table->eventPlayer(Raw));
        }

        private protected Level EventLevel()
        {
            return new Level(NativeApi.Table->eventLevel(Raw));
        }

        private protected Entity EventEntity()
        {
            return new Entity(NativeApi.Table->eventEntity(Raw));
        }

        private protected Entity? OptionalEntity()
        {
            return Entity.Wrap(NativeApi.Table->eventEntity(Raw));
        }

        private protected Entity? EventAttacker()
        {
            return Entity.Wrap(NativeApi.Table->eventAttacker(Raw));
        }

        private protected Entity? EventTarget()
        {
            return Entity.Wrap(NativeApi.Table->eventTarget(Raw));
        }

        private protected Item EventItem()
        {
            return Item.Borrow(NativeApi.Table->eventItem(Raw));
        }

        private protected BlockPos EventBlockPosition()
        {
            return BlockPos.From(NativeApi.Table->eventBlockPosition(Raw));
        }

        private protected string EventBlockName()
        {
            return NativeApi.Text(NativeApi.Table->eventBlockName(Raw));
        }

        private protected Vec3 EventPosition()
        {
            return Vec3.From(NativeApi.Table->eventPosition(Raw));
        }

        private protected Vec3 EventFrom()
        {
            return Vec3.From(NativeApi.Table->eventFrom(Raw));
        }

        private protected Vec3 EventTo()
        {
            return Vec3.From(NativeApi.Table->eventTo(Raw));
        }

        private protected void SetEventTo(Vec3 position)
        {
            NativeApi.Table->eventSetTo(Raw, position.ToNative());
        }

        private protected string EventMessage()
        {
            return NativeApi.Text(NativeApi.Table->eventMessage(Raw));
        }

        private protected void SetEventMessage(string message)
        {
            fixed (byte* text = NativeApi.Utf8(message))
            {
                NativeApi.Table->eventSetMessage(Raw, text);
            }
        }

        private protected double EventAmount()
        {
            return NativeApi.Table->eventAmount(Raw);
        }

        private protected void SetEventAmount(double amount)
        {
            NativeApi.Table->eventSetAmount(Raw, amount);
        }

        private protected string EventCause()
        {
            return NativeApi.Text(NativeApi.Table->eventCause(Raw));
        }

        private protected bool EventState()
        {
            return NativeApi.Table->eventState(Raw) != 0;
        }

        private protected double EventPreviousAmount()
        {
            return NativeApi.Table->eventPreviousAmount(Raw);
        }

        private protected string EventPreviousBlockName()
        {
            return NativeApi.Text(NativeApi.Table->eventPreviousBlockName(Raw));
        }

        private protected int EventChunkX()
        {
            return NativeApi.Table->eventChunkX(Raw);
        }

        private protected int EventChunkZ()
        {
            return NativeApi.Table->eventChunkZ(Raw);
        }

        private protected int EventSourceSlot()
        {
            return NativeApi.Table->eventSourceSlot(Raw);
        }

        private protected int EventDestinationSlot()
        {
            return NativeApi.Table->eventDestinationSlot(Raw);
        }

        private protected uint EventBlockFace()
        {
            return NativeApi.Table->eventBlockFace(Raw);
        }

        private protected IReadOnlyList<BlockPos> EventBlocks()
        {
            uint count = NativeApi.Table->eventBlockCount(Raw);
            List<BlockPos> blocks = new List<BlockPos>((int)count);
            for (uint index = 0; index < count; index++)
            {
                blocks.Add(BlockPos.From(NativeApi.Table->eventBlockAt(Raw, index)));
            }

            return blocks;
        }

        private protected Item EventResult()
        {
            return Item.Borrow(NativeApi.Table->eventResult(Raw));
        }

        private protected void SetEventResult(Item item)
        {
            NativeApi.Table->eventSetResult(Raw, item.Raw);
        }
    }
}
