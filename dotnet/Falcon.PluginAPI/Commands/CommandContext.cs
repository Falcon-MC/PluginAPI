using System.Collections.Generic;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class CommandContext
    {
        private readonly FalconCommandSender* _sender;

        internal CommandContext(FalconCommandSender* sender, IReadOnlyList<string> arguments)
        {
            _sender = sender;
            Arguments = arguments;
        }

        public IReadOnlyList<string> Arguments { get; }

        public nint Handle
        {
            get
            {
                return (nint)_sender;
            }
        }

        public string SenderName
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->senderName(_sender));
            }
        }

        public Player? Player
        {
            get
            {
                return Player.Wrap(NativeApi.Table->senderPlayer(_sender));
            }
        }

        public bool HasPermission(string node)
        {
            fixed (byte* text = NativeApi.Utf8(node))
            {
                return NativeApi.Table->senderHasPermission(_sender, text) != 0;
            }
        }

        public void Reply(string message)
        {
            fixed (byte* text = NativeApi.Utf8(message))
            {
                NativeApi.Table->senderSendMessage(_sender, text);
            }
        }
    }
}
