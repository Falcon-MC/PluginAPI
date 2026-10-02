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

        /// <summary>A dimension of a loaded world, or null when no world has that name.</summary>
        public static Level? GetLevel(string world, Dimension dimension)
        {
            fixed (byte* text = NativeApi.Utf8(world))
            {
                return Level.Wrap(NativeApi.Table->worldLevel(text, (uint)dimension));
            }
        }

        public static IReadOnlyList<string> Worlds
        {
            get
            {
                uint count = NativeApi.Table->serverWorldCount();
                List<string> names = new List<string>((int)count);
                for (uint index = 0; index < count; index++)
                {
                    names.Add(NativeApi.Text(NativeApi.Table->serverWorldName(index)));
                }

                return names;
            }
        }

        public static string DefaultWorld
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->serverDefaultWorldName());
            }
        }

        /// <summary>Loads a world from disk, creating it when <paramref name="create"/> is set. A null or empty seed picks a random one.</summary>
        public static bool LoadWorld(string name, bool create = false, string? seed = null)
        {
            fixed (byte* nameText = NativeApi.Utf8(name))
            fixed (byte* seedText = string.IsNullOrEmpty(seed) ? null : NativeApi.Utf8(seed))
            {
                return NativeApi.Table->serverLoadWorld(nameText, create ? 1 : 0, seedText) != 0;
            }
        }

        /// <summary>Saves and closes a world; fails for the default world and for worlds that still hold players.</summary>
        public static bool UnloadWorld(string name)
        {
            fixed (byte* text = NativeApi.Utf8(name))
            {
                return NativeApi.Table->serverUnloadWorld(text) != 0;
            }
        }
    }
}
