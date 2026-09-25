using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Events
    {
        private readonly nint _plugin;

        internal Events(nint plugin)
        {
            _plugin = plugin;
        }

        public ulong On<T>(Action<T> handler, EventPriority priority = EventPriority.Normal,
                           bool ignoreCancelled = false) where T : Event, IEvent<T>
        {
            ArgumentNullException.ThrowIfNull(handler);
            Action<nint> dispatch = handle =>
            {
                handler(T.Create(handle));
            };

            void* userData = PluginCallback.Keep(_plugin, dispatch);
            ulong subscription = NativeApi.Table->subscribe((FalconPlugin*)_plugin, T.EventType, (uint)priority,
                                                            ignoreCancelled ? 1 : 0, &Dispatch, userData);
            if (subscription == 0)
            {
                PluginCallback.Release(userData);
            }

            return subscription;
        }

        public void Off(ulong subscription)
        {
            NativeApi.Table->unsubscribe(subscription);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        internal static void Dispatch(FalconEvent* handle, void* userData)
        {
            PluginCallback.Run(userData, false, "An event handler threw an exception", (nint)handle,
                               static (target, state) =>
                               {
                                   ((Action<nint>)target)(state);
                                   return true;
                               }, false);
        }
    }
}
