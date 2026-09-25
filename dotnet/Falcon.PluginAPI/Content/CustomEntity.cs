using System;

namespace Falcon
{
    public sealed class CustomEntity
    {
        public string Identifier { get; set; } = string.Empty;

        public float Width { get; set; } = 0.6f;

        public float Height { get; set; } = 1.8f;

        public float MaxHealth { get; set; } = 20.0f;

        public bool Summonable { get; set; } = true;

        public Action<Entity>? OnTick { get; set; }

        public Func<Entity, Player, bool>? OnInteract { get; set; }
    }
}
