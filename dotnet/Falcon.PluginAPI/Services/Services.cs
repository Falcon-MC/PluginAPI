using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Services
    {
        [ThreadStatic]
        private static nint _response;

        private readonly nint _plugin;

        internal Services(nint plugin)
        {
            _plugin = plugin;
        }

        public bool Provide(string name, Func<string, string> handler)
        {
            ArgumentNullException.ThrowIfNull(handler);
            void* userData = PluginCallback.Keep(_plugin, handler);
            bool provided;
            fixed (byte* text = NativeApi.Utf8(name))
            {
                provided = NativeApi.Table->registerService((FalconPlugin*)_plugin, text, &Serve, userData) != 0;
            }

            if (!provided)
            {
                PluginCallback.Release(userData);
            }

            return provided;
        }

        public void Withdraw(string name)
        {
            fixed (byte* text = NativeApi.Utf8(name))
            {
                NativeApi.Table->unregisterService((FalconPlugin*)_plugin, text);
            }
        }

        public bool Has(string name)
        {
            fixed (byte* text = NativeApi.Utf8(name))
            {
                return NativeApi.Table->hasService(text) != 0;
            }
        }

        public string? Provider(string name)
        {
            fixed (byte* text = NativeApi.Utf8(name))
            {
                byte* plugin = NativeApi.Table->serviceProvider(text);
                if (plugin == null)
                {
                    return null;
                }

                return NativeApi.Text(plugin);
            }
        }

        public string? Call(string name, string request = "")
        {
            int found = 0;
            byte* response;
            fixed (byte* nameText = NativeApi.Utf8(name))
            fixed (byte* requestText = NativeApi.Utf8(request))
            {
                response = NativeApi.Table->callService(nameText, requestText, &found);
            }

            if (found == 0)
            {
                return null;
            }

            return NativeApi.Text(response);
        }

        public ulong On(string name, Action<CustomEvent> handler, EventPriority priority = EventPriority.Normal,
                        bool ignoreCancelled = false)
        {
            ArgumentNullException.ThrowIfNull(handler);
            Action<nint> dispatch = handle =>
            {
                handler(CustomEvent.Create(handle));
            };

            void* userData = PluginCallback.Keep(_plugin, dispatch);
            ulong subscription;
            fixed (byte* text = NativeApi.Utf8(name))
            {
                subscription = NativeApi.Table->subscribeCustomEvent((FalconPlugin*)_plugin, text, (uint)priority,
                                                                     ignoreCancelled ? 1 : 0, &Events.Dispatch,
                                                                     userData);
            }

            if (subscription == 0)
            {
                PluginCallback.Release(userData);
            }

            return subscription;
        }

        public CustomEventResult Fire(string name, string data = "", bool cancellable = false)
        {
            int cancelled = 0;
            byte* result;
            fixed (byte* nameText = NativeApi.Utf8(name))
            fixed (byte* dataText = NativeApi.Utf8(data))
            {
                result = NativeApi.Table->fireCustomEvent((FalconPlugin*)_plugin, nameText, dataText,
                                                          cancellable ? 1 : 0, &cancelled);
            }

            return new CustomEventResult(cancelled != 0, NativeApi.Text(result));
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static byte* Serve(byte* request, void* userData)
        {
            string answer = PluginCallback.Run(userData, false, "A service handler threw an exception", (nint)request,
                                               static (target, state) =>
                                               {
                                                   string text = NativeApi.Text((byte*)state);
                                                   return ((Func<string, string>)target)(text) ?? string.Empty;
                                               }, string.Empty);
            return Hold(answer);
        }

        private static byte* Hold(string value)
        {
            if (_response != 0)
            {
                Marshal.FreeCoTaskMem(_response);
                _response = 0;
            }

            _response = Marshal.StringToCoTaskMemUTF8(value);
            return (byte*)_response;
        }
    }
}
