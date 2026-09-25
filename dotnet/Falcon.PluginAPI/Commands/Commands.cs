using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Commands
    {
        private readonly nint _plugin;

        internal Commands(nint plugin)
        {
            _plugin = plugin;
        }

        public bool Add(string name, string description, string usage, Func<CommandContext, bool> handler,
                        CommandPermission permission = CommandPermission.Operator)
        {
            ArgumentNullException.ThrowIfNull(handler);
            void* userData = PluginCallback.Keep(_plugin, handler);
            bool added;
            fixed (byte* nameText = NativeApi.Utf8(name))
            fixed (byte* descriptionText = NativeApi.Utf8(description))
            fixed (byte* usageText = NativeApi.Utf8(usage))
            {
                FalconCommandDescriptor descriptor = default;
                descriptor.name = nameText;
                descriptor.description = descriptionText;
                descriptor.usage = usageText;
                descriptor.permission = (uint)permission;
                descriptor.handler = &Dispatch;
                descriptor.userData = userData;
                added = NativeApi.Table->registerCommand((FalconPlugin*)_plugin, &descriptor) != 0;
            }

            if (!added)
            {
                PluginCallback.Release(userData);
            }

            return added;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static int Dispatch(FalconCommandSender* sender, byte** arguments, uint argumentCount, void* userData)
        {
            bool handled = PluginCallback.Run(userData, false, "A command handler threw an exception",
                                              ((nint)sender, (nint)arguments, argumentCount),
                                              static (target, state) =>
                                              {
                                                  byte** raw = (byte**)state.Item2;
                                                  string[] values = new string[state.Item3];
                                                  for (uint index = 0; index < state.Item3; index++)
                                                  {
                                                      values[index] = NativeApi.Text(raw[index]);
                                                  }

                                                  CommandContext context =
                                                      new CommandContext((FalconCommandSender*)state.Item1, values);
                                                  return ((Func<CommandContext, bool>)target)(context);
                                              }, false);
            return handled ? 1 : 0;
        }
    }
}
