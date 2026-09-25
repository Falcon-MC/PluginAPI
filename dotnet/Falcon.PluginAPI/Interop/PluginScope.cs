using System;

namespace Falcon.Interop
{
    internal static unsafe class PluginScope
    {
        [ThreadStatic]
        private static nint _current;

        internal static nint Current
        {
            get
            {
                return _current;
            }
        }

        internal static void Set(nint plugin)
        {
            _current = plugin;
        }

        internal static FalconPlugin* Require()
        {
            if (_current == 0)
            {
                throw new InvalidOperationException("This call must be made from code the server runs for a plugin");
            }

            return (FalconPlugin*)_current;
        }
    }
}
