using System.IO;
using Falcon;
using Hello.Commands;
using Hello.Content;
using Hello.Listeners;
using Hello.Storage;
using Hello.Tasks;

namespace Hello
{
    public sealed class HelloPlugin : Plugin
    {
        private readonly JoinStore _joins = new JoinStore();

        public override void OnLoad()
        {
            ThunderWand.RegisterTo(this);
        }

        public override bool OnEnable()
        {
            _joins.Load(Path.Combine(DataFolder, "joins.txt"));

            JoinListener.RegisterTo(this, _joins);
            ChatListener.RegisterTo(this);
            BlockBreakListener.RegisterTo(this);
            JoinsCommand.RegisterTo(this, _joins);
            GiveDiamondCommand.RegisterTo(this);
            AnnouncementTask.Start(this);

            Logger.Info("HelloPlugin enabled");
            return true;
        }

        public override void OnDisable()
        {
            _joins.Save();
            Logger.Info("HelloPlugin disabled");
        }
    }
}
