using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;

namespace Falcon.Hosting
{
    internal sealed class PluginLoadContext : AssemblyLoadContext
    {
        private readonly Assembly _api;
        private readonly IReadOnlyList<PluginLoadContext> _dependencies;
        private readonly string _directory;
        private readonly AssemblyDependencyResolver? _resolver;

        internal PluginLoadContext(string name, string assemblyPath, Assembly api,
                                   IReadOnlyList<PluginLoadContext> dependencies) : base(name)
        {
            _api = api;
            _dependencies = dependencies;
            _directory = Path.GetDirectoryName(assemblyPath) ?? string.Empty;
            _resolver = CreateResolver(assemblyPath);
        }

        protected override Assembly? Load(AssemblyName assemblyName)
        {
            if (IsApi(assemblyName))
            {
                return _api;
            }

            HashSet<PluginLoadContext> visited = new HashSet<PluginLoadContext> { this };
            Assembly? shared = FromDependencies(assemblyName, visited);
            if (shared != null)
            {
                return shared;
            }

            string? path = ResolvePath(assemblyName);
            if (path == null)
            {
                return null;
            }

            return LoadFromAssemblyPath(path);
        }

        protected override nint LoadUnmanagedDll(string unmanagedDllName)
        {
            string? path = _resolver?.ResolveUnmanagedDllToPath(unmanagedDllName);
            if (path == null)
            {
                return 0;
            }

            return LoadUnmanagedDllFromPath(path);
        }

        private Assembly? Provide(AssemblyName assemblyName, HashSet<PluginLoadContext> visited)
        {
            if (!visited.Add(this))
            {
                return null;
            }

            foreach (Assembly loaded in Assemblies)
            {
                if (Satisfies(loaded.GetName(), assemblyName))
                {
                    return loaded;
                }
            }

            Assembly? shared = FromDependencies(assemblyName, visited);
            if (shared != null)
            {
                return shared;
            }

            string? path = ResolvePath(assemblyName);
            if (path == null)
            {
                return null;
            }

            return LoadFromAssemblyPath(path);
        }

        private Assembly? FromDependencies(AssemblyName assemblyName, HashSet<PluginLoadContext> visited)
        {
            foreach (PluginLoadContext dependency in _dependencies)
            {
                Assembly? shared = dependency.Provide(assemblyName, visited);
                if (shared != null)
                {
                    return shared;
                }
            }

            return null;
        }

        private string? ResolvePath(AssemblyName assemblyName)
        {
            if (string.IsNullOrEmpty(assemblyName.Name))
            {
                return null;
            }

            string? path = _resolver?.ResolveAssemblyToPath(assemblyName);
            if (path == null)
            {
                string local = Path.Combine(_directory, assemblyName.Name + ".dll");
                if (!File.Exists(local))
                {
                    return null;
                }

                path = local;
            }

            if (!Satisfies(AssemblyName.GetAssemblyName(path), assemblyName))
            {
                return null;
            }

            return path;
        }

        private bool IsApi(AssemblyName assemblyName)
        {
            return string.Equals(assemblyName.Name, _api.GetName().Name, StringComparison.OrdinalIgnoreCase);
        }

        private static bool Satisfies(AssemblyName candidate, AssemblyName requested)
        {
            if (!string.Equals(candidate.Name, requested.Name, StringComparison.OrdinalIgnoreCase))
            {
                return false;
            }

            return requested.Version == null || candidate.Version == null || candidate.Version >= requested.Version;
        }

        private static AssemblyDependencyResolver? CreateResolver(string assemblyPath)
        {
            try
            {
                return new AssemblyDependencyResolver(assemblyPath);
            }
            catch (InvalidOperationException)
            {
                return null;
            }
        }
    }
}
