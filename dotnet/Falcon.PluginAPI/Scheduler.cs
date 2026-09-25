using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Scheduler
    {
        private readonly nint _plugin;

        internal Scheduler(nint plugin)
        {
            _plugin = plugin;
        }

        public ulong Delay(ulong ticks, Action task)
        {
            ArgumentNullException.ThrowIfNull(task);
            void* userData = PluginCallback.Keep(_plugin, task);
            ulong id = NativeApi.Table->scheduleTask((FalconPlugin*)_plugin, &RunOnce, userData, ticks, 0);
            if (id == 0)
            {
                PluginCallback.Release(userData);
            }

            return id;
        }

        public ulong Repeat(ulong periodTicks, Action task)
        {
            ArgumentNullException.ThrowIfNull(task);
            void* userData = PluginCallback.Keep(_plugin, task);
            ulong id = NativeApi.Table->scheduleTask((FalconPlugin*)_plugin, &RunRepeating, userData, periodTicks,
                                                     periodTicks);
            if (id == 0)
            {
                PluginCallback.Release(userData);
            }

            return id;
        }

        public ulong Async(Action work, Action? done = null)
        {
            ArgumentNullException.ThrowIfNull(work);
            void* userData = PluginCallback.Keep(_plugin, new AsyncJob(work, done));
            ulong id = NativeApi.Table->runAsync((FalconPlugin*)_plugin, &RunWork, &RunDone, userData);
            if (id == 0)
            {
                PluginCallback.Release(userData);
            }

            return id;
        }

        public void Cancel(ulong task)
        {
            NativeApi.Table->cancelTask(task);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void RunOnce(void* userData)
        {
            PluginCallback.Run(userData, true, "A task threw an exception", 0, static (target, state) =>
            {
                ((Action)target)();
                return true;
            }, false);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void RunRepeating(void* userData)
        {
            PluginCallback.Run(userData, false, "A task threw an exception", 0, static (target, state) =>
            {
                ((Action)target)();
                return true;
            }, false);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void RunWork(void* userData)
        {
            PluginCallback.Run(userData, false, "An asynchronous task threw an exception", 0, static (target, state) =>
            {
                ((AsyncJob)target).Work();
                return true;
            }, false);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void RunDone(void* userData)
        {
            PluginCallback.Run(userData, true, "An asynchronous callback threw an exception", 0,
                               static (target, state) =>
                               {
                                   Action? done = ((AsyncJob)target).Done;
                                   if (done != null)
                                   {
                                       done();
                                   }

                                   return true;
                               }, false);
        }

        private sealed class AsyncJob
        {
            internal AsyncJob(Action work, Action? done)
            {
                Work = work;
                Done = done;
            }

            internal Action Work { get; }

            internal Action? Done { get; }
        }
    }
}
