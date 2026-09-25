using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

namespace Falcon.Interop
{
    public static unsafe class NativeApi
    {
        private static FalconServerApi* _table;

        public static FalconServerApi* Table
        {
            get
            {
                return _table;
            }
        }

        internal static bool Attach(FalconServerApi* table, FalconPlugin* plugin)
        {
            if (table == null)
            {
                return false;
            }

            bool compatible = table->versionMajor == FalconConstants.ApiVersionMajor
                              && table->size >= (uint)sizeof(FalconServerApi);
            if (!compatible)
            {
                string message = "Falcon.PluginAPI " + FalconConstants.ApiVersionMajor + "."
                                 + FalconConstants.ApiVersionMinor + " needs a server with the same API major "
                                 + "version and at least the same minor version, the server provides "
                                 + table->versionMajor + "." + table->versionMinor;
                Write(table, plugin, FalconConstants.LogError, message);
                return false;
            }

            _table = table;
            return true;
        }

        internal static string Text(byte* value)
        {
            if (value == null)
            {
                return string.Empty;
            }

            return Marshal.PtrToStringUTF8((nint)value) ?? string.Empty;
        }

        internal static byte[] Utf8(string? value)
        {
            string text = value ?? string.Empty;
            byte[] bytes = new byte[Encoding.UTF8.GetByteCount(text) + 1];
            Encoding.UTF8.GetBytes(text, 0, text.Length, bytes, 0);
            return bytes;
        }

        internal static byte[]? Nullable(string? value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return null;
            }

            return Utf8(value);
        }

        internal static void Log(nint plugin, uint level, string message)
        {
            Write(_table, (FalconPlugin*)plugin, level, message);
        }

        internal static void Report(nint plugin, string where, Exception exception)
        {
            try
            {
                Exception cause = exception;
                if (cause is TargetInvocationException invocation && invocation.InnerException != null)
                {
                    cause = invocation.InnerException;
                }

                Log(plugin, FalconConstants.LogError, where + ": " + cause);
            }
            catch (Exception)
            {
            }
        }

        private static void Write(FalconServerApi* table, FalconPlugin* plugin, uint level, string message)
        {
            if (table == null || plugin == null || table->log == null)
            {
                return;
            }

            fixed (byte* text = Utf8(message))
            {
                table->log(plugin, level, text);
            }
        }
    }
}
