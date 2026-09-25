using System;
using System.Runtime.InteropServices;

namespace Falcon.Interop
{
    internal sealed unsafe class PluginCallback
    {
        private PluginCallback(nint plugin, object target)
        {
            Plugin = plugin;
            Target = target;
        }

        internal nint Plugin { get; }

        internal object Target { get; }

        internal static void* Keep(nint plugin, object target)
        {
            GCHandle handle = GCHandle.Alloc(new PluginCallback(plugin, target));
            return (void*)GCHandle.ToIntPtr(handle);
        }

        internal static void Release(void* userData)
        {
            GCHandle.FromIntPtr((nint)userData).Free();
        }

        internal static TResult Run<TState, TResult>(void* userData, bool release, string where, TState state,
                                                     Func<object, TState, TResult> body, TResult fallback)
        {
            nint previous = PluginScope.Current;
            nint plugin = previous;
            try
            {
                GCHandle handle = GCHandle.FromIntPtr((nint)userData);
                PluginCallback callback = (PluginCallback)handle.Target!;
                if (release)
                {
                    handle.Free();
                }

                plugin = callback.Plugin;
                PluginScope.Set(plugin);
                return body(callback.Target, state);
            }
            catch (Exception exception)
            {
                NativeApi.Report(plugin, where, exception);
                return fallback;
            }
            finally
            {
                PluginScope.Set(previous);
            }
        }
    }
}
