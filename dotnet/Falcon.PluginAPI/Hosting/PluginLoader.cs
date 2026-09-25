using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon.Hosting
{
    [EditorBrowsable(EditorBrowsableState.Never)]
    public static unsafe class PluginLoader
    {
        private static readonly Dictionary<string, PluginLoadContext> Contexts =
            new Dictionary<string, PluginLoadContext>(StringComparer.OrdinalIgnoreCase);

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        public static int Load(FalconServerApi* api, FalconPlugin* plugin, FalconPluginCallbacks* callbacks,
                               byte* assemblyPath, byte* mainClass, byte** dependencies, uint dependencyCount)
        {
            nint previous = PluginScope.Current;
            try
            {
                if (callbacks == null || !NativeApi.Attach(api, plugin))
                {
                    return 0;
                }

                PluginScope.Set((nint)plugin);
                List<string> names = new List<string>();
                for (uint index = 0; index < dependencyCount; index++)
                {
                    names.Add(NativeApi.Text(dependencies[index]));
                }

                return Start(plugin, callbacks, NativeApi.Text(assemblyPath), NativeApi.Text(mainClass), names) ? 1 : 0;
            }
            catch (Exception exception)
            {
                NativeApi.Report((nint)plugin, "Could not load the plugin", exception);
                return 0;
            }
            finally
            {
                PluginScope.Set(previous);
            }
        }

        private static bool Start(FalconPlugin* plugin, FalconPluginCallbacks* callbacks, string assemblyPath,
                                  string mainClass, List<string> dependencyNames)
        {
            string name = NativeApi.Text(NativeApi.Table->pluginName(plugin));
            List<PluginLoadContext> dependencies = new List<PluginLoadContext>();
            foreach (string dependencyName in dependencyNames)
            {
                PluginLoadContext? dependency;
                if (Contexts.TryGetValue(dependencyName, out dependency))
                {
                    dependencies.Add(dependency);
                }
            }

            Assembly api = typeof(PluginLoader).Assembly;
            PluginLoadContext context = new PluginLoadContext(name, assemblyPath, api, dependencies);
            Assembly assembly = context.LoadFromAssemblyPath(assemblyPath);
            if (!AcceptsApi(plugin, assembly, api))
            {
                return false;
            }

            Type? type = assembly.GetType(mainClass, false);
            if (type == null)
            {
                Fail(plugin, "The class " + mainClass + " was not found in " + Path.GetFileName(assemblyPath));
                return false;
            }

            if (type.IsAbstract || !typeof(Plugin).IsAssignableFrom(type))
            {
                Fail(plugin, mainClass + " must be a concrete class that extends Falcon.Plugin");
                return false;
            }

            Plugin instance = (Plugin)Activator.CreateInstance(type, true)!;
            callbacks->onLoad = &OnLoad;
            callbacks->onEnable = &OnEnable;
            callbacks->onDisable = &OnDisable;
            callbacks->userData = PluginCallback.Keep((nint)plugin, instance);
            Contexts[name] = context;
            return true;
        }

        private static bool AcceptsApi(FalconPlugin* plugin, Assembly assembly, Assembly api)
        {
            AssemblyName shared = api.GetName();
            foreach (AssemblyName reference in assembly.GetReferencedAssemblies())
            {
                if (!string.Equals(reference.Name, shared.Name, StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                if (reference.Version != null && shared.Version != null && reference.Version > shared.Version)
                {
                    Fail(plugin, "It was built against Falcon.PluginAPI " + reference.Version + " but the server "
                                 + "loaded Falcon.PluginAPI " + shared.Version + " from " + api.Location);
                    return false;
                }
            }

            return true;
        }

        private static void Fail(FalconPlugin* plugin, string message)
        {
            NativeApi.Log((nint)plugin, FalconConstants.LogError, message);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void OnLoad(void* userData)
        {
            PluginCallback.Run(userData, false, "onLoad threw an exception", 0, static (target, state) =>
            {
                ((Plugin)target).OnLoad();
                return true;
            }, false);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static int OnEnable(void* userData)
        {
            bool enabled = PluginCallback.Run(userData, false, "onEnable threw an exception", 0,
                                              static (target, state) =>
                                              {
                                                  return ((Plugin)target).OnEnable();
                                              }, false);
            return enabled ? 1 : 0;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void OnDisable(void* userData)
        {
            PluginCallback.Run(userData, false, "onDisable threw an exception", 0, static (target, state) =>
            {
                ((Plugin)target).OnDisable();
                return true;
            }, false);
        }
    }
}
