using System;
using System.IO;
using Falcon.Interop;

namespace Falcon
{
    public abstract unsafe class Plugin
    {
        private readonly nint _handle;
        private Config? _config;

        protected Plugin()
        {
            _handle = PluginScope.Current;
            if (_handle == 0)
            {
                throw new InvalidOperationException("Plugins are created by the server, not by other code");
            }

            Logger = new Logger(_handle);
            Events = new Events(_handle);
            Commands = new Commands(_handle);
            Scheduler = new Scheduler(_handle);
            Permissions = new Permissions(_handle);
            Content = new Content(_handle);
            Services = new Services(_handle);
        }

        public nint Handle
        {
            get
            {
                return _handle;
            }
        }

        public string Name
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->pluginName((FalconPlugin*)_handle));
            }
        }

        public string DataFolder
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->pluginDataFolder((FalconPlugin*)_handle));
            }
        }

        public Logger Logger { get; }

        public Events Events { get; }

        public Commands Commands { get; }

        public Scheduler Scheduler { get; }

        public Permissions Permissions { get; }

        public Content Content { get; }

        public Services Services { get; }

        public Config Config
        {
            get
            {
                if (_config == null)
                {
                    _config = new Config(Path.Combine(DataFolder, "config.yml"));
                    _config.Load();
                }

                return _config;
            }
        }

        public virtual void OnLoad()
        {
        }

        public virtual bool OnEnable()
        {
            return true;
        }

        public virtual void OnDisable()
        {
        }

        public void SaveConfig()
        {
            Config.Save();
        }
    }
}
