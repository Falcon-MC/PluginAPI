using System;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Player : IEquatable<Player>
    {
        private readonly FalconPlayer* _handle;

        internal Player(FalconPlayer* handle)
        {
            _handle = handle;
        }

        public nint Handle
        {
            get
            {
                return (nint)_handle;
            }
        }

        public bool IsValid
        {
            get
            {
                return _handle != null;
            }
        }

        public string Name
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->playerName(_handle));
            }
        }

        public bool IsOperator
        {
            get
            {
                return NativeApi.Table->playerIsOperator(_handle) != 0;
            }
            set
            {
                NativeApi.Table->playerSetOperator(_handle, value ? 1 : 0);
            }
        }

        public Entity Entity
        {
            get
            {
                return new Entity(NativeApi.Table->playerEntity(_handle));
            }
        }

        public GameMode GameMode
        {
            get
            {
                return (GameMode)NativeApi.Table->playerGameMode(_handle);
            }
            set
            {
                NativeApi.Table->playerSetGameMode(_handle, (uint)value);
            }
        }

        public string Xuid
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->playerXuid(_handle));
            }
        }

        public string Uuid
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->playerUuid(_handle));
            }
        }

        public string Address
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->playerAddress(_handle));
            }
        }

        public float Food
        {
            get
            {
                return NativeApi.Table->playerFood(_handle);
            }
            set
            {
                NativeApi.Table->playerSetFood(_handle, value);
            }
        }

        public int XpLevel
        {
            get
            {
                return NativeApi.Table->playerXpLevel(_handle);
            }
            set
            {
                NativeApi.Table->playerSetXpLevel(_handle, value);
            }
        }

        public PlayerInventory Inventory
        {
            get
            {
                return new PlayerInventory(_handle);
            }
        }

        internal FalconPlayer* Raw
        {
            get
            {
                return _handle;
            }
        }

        public static bool operator ==(Player? left, Player? right)
        {
            if (left is null)
            {
                return right is null;
            }

            return left.Equals(right);
        }

        public static bool operator !=(Player? left, Player? right)
        {
            return !(left == right);
        }

        public void SendMessage(string message)
        {
            fixed (byte* text = NativeApi.Utf8(message))
            {
                NativeApi.Table->playerSendMessage(_handle, text);
            }
        }

        public void Kick(string reason)
        {
            fixed (byte* text = NativeApi.Utf8(reason))
            {
                NativeApi.Table->playerKick(_handle, text);
            }
        }

        public void SendTitle(string title, string subtitle = "")
        {
            fixed (byte* titleText = NativeApi.Utf8(title))
            fixed (byte* subtitleText = NativeApi.Utf8(subtitle))
            {
                NativeApi.Table->playerSendTitle(_handle, titleText, subtitleText);
            }
        }

        public void SendActionBar(string message)
        {
            fixed (byte* text = NativeApi.Utf8(message))
            {
                NativeApi.Table->playerSendActionBar(_handle, text);
            }
        }

        public bool HasPermission(string node)
        {
            fixed (byte* text = NativeApi.Utf8(node))
            {
                return NativeApi.Table->playerHasPermission(_handle, text) != 0;
            }
        }

        public void SetPermission(string node, bool value = true)
        {
            FalconPlugin* plugin = PluginScope.Require();
            fixed (byte* text = NativeApi.Utf8(node))
            {
                NativeApi.Table->playerSetPermission(plugin, _handle, text, value ? 1 : 0);
            }
        }

        public void UnsetPermission(string node)
        {
            FalconPlugin* plugin = PluginScope.Require();
            fixed (byte* text = NativeApi.Utf8(node))
            {
                NativeApi.Table->playerUnsetPermission(plugin, _handle, text);
            }
        }

        public bool SendPacket(uint packetId, ReadOnlySpan<byte> data)
        {
            fixed (byte* bytes = data)
            {
                return NativeApi.Table->playerSendPacket(_handle, packetId, bytes, (uint)data.Length) != 0;
            }
        }

        public bool Equals(Player? other)
        {
            return other is not null && other._handle == _handle;
        }

        public override bool Equals(object? other)
        {
            return Equals(other as Player);
        }

        public override int GetHashCode()
        {
            return ((nint)_handle).GetHashCode();
        }

        internal static Player? Wrap(FalconPlayer* handle)
        {
            if (handle == null)
            {
                return null;
            }

            return new Player(handle);
        }
    }
}
