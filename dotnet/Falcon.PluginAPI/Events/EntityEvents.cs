using Falcon.Interop;

namespace Falcon
{
    public sealed class EntityDamageEvent : Event, IEvent<EntityDamageEvent>
    {
        private EntityDamageEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventEntityDamage;
            }
        }

        public Entity Entity
        {
            get
            {
                return EventEntity();
            }
        }

        public Entity? Attacker
        {
            get
            {
                return EventAttacker();
            }
        }

        public double Amount
        {
            get
            {
                return EventAmount();
            }
            set
            {
                SetEventAmount(value);
            }
        }

        public string Cause
        {
            get
            {
                return EventCause();
            }
        }

        public static EntityDamageEvent Create(nint handle)
        {
            return new EntityDamageEvent(handle);
        }
    }

    public sealed class EntityDeathEvent : Event, IEvent<EntityDeathEvent>
    {
        private EntityDeathEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventEntityDeath;
            }
        }

        public Entity Entity
        {
            get
            {
                return EventEntity();
            }
        }

        public static EntityDeathEvent Create(nint handle)
        {
            return new EntityDeathEvent(handle);
        }
    }
}
