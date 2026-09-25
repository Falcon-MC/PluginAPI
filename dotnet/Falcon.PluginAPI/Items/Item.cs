using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Item : IDisposable
    {
        private FalconItem* _handle;
        private bool _owned;

        private Item(FalconItem* handle, bool owned)
        {
            _handle = handle;
            _owned = owned;
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

        public bool IsOwned
        {
            get
            {
                return _owned;
            }
        }

        public bool IsEmpty
        {
            get
            {
                return _handle == null || NativeApi.Table->itemIsEmpty(_handle) != 0;
            }
        }

        public string Identifier
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->itemIdentifier(Raw));
            }
        }

        public uint Count
        {
            get
            {
                return NativeApi.Table->itemCount(Raw);
            }
            set
            {
                NativeApi.Table->itemSetCount(Raw, value);
            }
        }

        public int Damage
        {
            get
            {
                return NativeApi.Table->itemDamage(Raw);
            }
            set
            {
                NativeApi.Table->itemSetDamage(Raw, value);
            }
        }

        public string CustomName
        {
            get
            {
                return NativeApi.Text(NativeApi.Table->itemCustomName(Raw));
            }
            set
            {
                fixed (byte* name = NativeApi.Utf8(value))
                {
                    NativeApi.Table->itemSetCustomName(Raw, name);
                }
            }
        }

        public IReadOnlyList<string> Lore
        {
            get
            {
                FalconItem* handle = Raw;
                uint count = NativeApi.Table->itemLoreCount(handle);
                List<string> lines = new List<string>((int)count);
                for (uint index = 0; index < count; index++)
                {
                    lines.Add(NativeApi.Text(NativeApi.Table->itemLore(handle, index)));
                }

                return lines;
            }
            set
            {
                SetLore(value);
            }
        }

        internal FalconItem* Raw
        {
            get
            {
                if (_handle == null)
                {
                    throw new ObjectDisposedException(nameof(Item));
                }

                return _handle;
            }
        }

        public static Item? Create(string identifier, uint count = 1)
        {
            fixed (byte* name = NativeApi.Utf8(identifier))
            {
                return Adopt(NativeApi.Table->itemCreate(name, count));
            }
        }

        public Item? Copy()
        {
            if (_handle == null)
            {
                return null;
            }

            return Adopt(NativeApi.Table->itemCopy(_handle));
        }

        public int GetEnchantmentLevel(uint enchantment)
        {
            return NativeApi.Table->itemEnchantmentLevel(Raw, enchantment);
        }

        public void SetEnchantmentLevel(uint enchantment, int level)
        {
            NativeApi.Table->itemSetEnchantmentLevel(Raw, enchantment, level);
        }

        public void Dispose()
        {
            if (_owned && _handle != null)
            {
                NativeApi.Table->itemDestroy(_handle);
            }

            _handle = null;
            _owned = false;
        }

        internal static Item? Adopt(FalconItem* handle)
        {
            if (handle == null)
            {
                return null;
            }

            return new Item(handle, true);
        }

        internal static Item Borrow(FalconItem* handle)
        {
            return new Item(handle, false);
        }

        private void SetLore(IReadOnlyList<string> lines)
        {
            FalconItem* handle = Raw;
            nint[] buffers = new nint[lines.Count];
            try
            {
                for (int index = 0; index < lines.Count; index++)
                {
                    buffers[index] = Marshal.StringToCoTaskMemUTF8(lines[index] ?? string.Empty);
                }

                fixed (nint* values = buffers)
                {
                    NativeApi.Table->itemSetLore(handle, (byte**)values, (uint)buffers.Length);
                }
            }
            finally
            {
                foreach (nint buffer in buffers)
                {
                    if (buffer != 0)
                    {
                        Marshal.FreeCoTaskMem(buffer);
                    }
                }
            }
        }
    }
}
