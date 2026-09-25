using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class PlayerInventory
    {
        private readonly FalconPlayer* _player;

        internal PlayerInventory(FalconPlayer* player)
        {
            _player = player;
        }

        public uint Size
        {
            get
            {
                return NativeApi.Table->playerInventorySize(_player);
            }
        }

        public uint SelectedSlot
        {
            get
            {
                return NativeApi.Table->playerSelectedSlot(_player);
            }
            set
            {
                NativeApi.Table->playerSetSelectedSlot(_player, value);
            }
        }

        public Item? HeldItem
        {
            get
            {
                return GetItem(SelectedSlot);
            }
        }

        public Item? Offhand
        {
            get
            {
                return Item.Adopt(NativeApi.Table->playerOffhandItem(_player));
            }
        }

        public Item? GetItem(uint slot)
        {
            return Item.Adopt(NativeApi.Table->playerInventoryItem(_player, slot));
        }

        public void SetItem(uint slot, Item item)
        {
            NativeApi.Table->playerSetInventoryItem(_player, slot, item.Raw);
        }

        public bool Give(Item item)
        {
            return NativeApi.Table->playerGiveItem(_player, item.Raw) != 0;
        }

        public Item? GetArmor(ArmorSlot slot)
        {
            return Item.Adopt(NativeApi.Table->playerArmorItem(_player, (uint)slot));
        }

        public void SetArmor(ArmorSlot slot, Item item)
        {
            NativeApi.Table->playerSetArmorItem(_player, (uint)slot, item.Raw);
        }

        public void SetOffhand(Item item)
        {
            NativeApi.Table->playerSetOffhandItem(_player, item.Raw);
        }

        public void Clear()
        {
            NativeApi.Table->playerClearInventory(_player);
        }
    }
}
