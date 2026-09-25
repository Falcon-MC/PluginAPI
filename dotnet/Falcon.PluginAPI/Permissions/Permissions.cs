using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Permissions
    {
        private readonly nint _plugin;

        internal Permissions(nint plugin)
        {
            _plugin = plugin;
        }

        public bool Add(string node, PermissionDefault defaultValue = PermissionDefault.Operator)
        {
            fixed (byte* text = NativeApi.Utf8(node))
            {
                return NativeApi.Table->registerPermission((FalconPlugin*)_plugin, text, (uint)defaultValue) != 0;
            }
        }
    }
}
