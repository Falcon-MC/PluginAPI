using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class CustomEvent : Event, IEvent<CustomEvent>
    {
        private CustomEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventCustom;
            }
        }

        public string Name
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventName(Raw));
            }
        }

        public string Data
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->eventData(Raw));
            }
            set
            {
                fixed (byte* text = NativeApi.Utf8(value))
                {
                    NativeApi.Table->eventSetData(Raw, text);
                }
            }
        }

        public string Source
        {
            get
            {
                return EventCause();
            }
        }

        public static CustomEvent Create(nint handle)
        {
            return new CustomEvent(handle);
        }
    }
}
