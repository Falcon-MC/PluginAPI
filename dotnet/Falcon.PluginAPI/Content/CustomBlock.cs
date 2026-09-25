using System;

namespace Falcon
{
    public sealed class CustomBlock
    {
        public string Identifier { get; set; } = string.Empty;

        public string DisplayName { get; set; } = string.Empty;

        public string Texture { get; set; } = string.Empty;

        public string CreativeCategory { get; set; } = string.Empty;

        public float DestroyTime { get; set; } = 1.0f;

        public float ExplosionResistance { get; set; } = 1.0f;

        public uint LightEmission { get; set; }

        public float Friction { get; set; } = 0.6f;

        public string Drop { get; set; } = string.Empty;

        public Func<Player, BlockPos, uint, bool>? OnInteract { get; set; }

        public Action<Player, BlockPos>? OnBreak { get; set; }
    }
}
