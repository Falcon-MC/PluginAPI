using System.Collections.Generic;
using Falcon.Interop;

namespace Falcon
{
    public static unsafe class Server
    {
        public static string Version
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->serverVersion());
            }
        }

        public static IReadOnlyList<Player> OnlinePlayers
        {
            get
            {
                uint count = NativeApi.Table->onlinePlayerCount();
                List<Player> players = new List<Player>((int)count);
                for (uint index = 0; index < count; index++)
                {
                    Player? player = Player.Wrap(NativeApi.Table->onlinePlayer(index));
                    if (player != null)
                    {
                        players.Add(player);
                    }
                }

                return players;
            }
        }

        public static Player? FindPlayer(string name)
        {
            fixed (byte* text = NativeApi.Utf8(name))
            {
                return Player.Wrap(NativeApi.Table->findPlayer(text));
            }
        }

        public static void Broadcast(string message)
        {
            fixed (byte* text = NativeApi.Utf8(message))
            {
                NativeApi.Table->broadcastMessage(text);
            }
        }

        public static Level? GetLevel(Dimension dimension)
        {
            return Level.Wrap(NativeApi.Table->serverLevel((uint)dimension));
        }
    }
}
