using Falcon.Interop;

namespace Falcon
{
    public sealed class Logger
    {
        private readonly nint _plugin;

        internal Logger(nint plugin)
        {
            _plugin = plugin;
        }

        public void Info(string message)
        {
            NativeApi.Log(_plugin, FalconConstants.LogInfo, message);
        }

        public void Warning(string message)
        {
            NativeApi.Log(_plugin, FalconConstants.LogWarning, message);
        }

        public void Error(string message)
        {
            NativeApi.Log(_plugin, FalconConstants.LogError, message);
        }
    }
}
