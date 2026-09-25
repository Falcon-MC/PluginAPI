using System;

namespace Falcon
{
    public sealed class CustomItem
    {
        public string Identifier { get; set; } = string.Empty;

        public string DisplayName { get; set; } = string.Empty;

        public string Icon { get; set; } = string.Empty;

        public string CreativeCategory { get; set; } = string.Empty;

        public uint MaxStackSize { get; set; } = 64;

        public uint MaxDurability { get; set; }

        public bool HandEquipped { get; set; }

        public Func<Player, Item, bool>? OnUse { get; set; }

        public Func<Player, Item, BlockPos, uint, bool>? OnUseOnBlock { get; set; }
    }
}
