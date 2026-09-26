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

    public sealed class EntitySpawnEvent : Event, IEvent<EntitySpawnEvent>
    {
        private EntitySpawnEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventEntitySpawn;
            }
        }

        public Entity Entity
        {
            get
            {
                return EventEntity();
            }
        }

        public Level Level
        {
            get
            {
                return EventLevel();
            }
        }

        public Vec3 Position
        {
            get
            {
                return EventPosition();
            }
        }

        public static EntitySpawnEvent Create(nint handle)
        {
            return new EntitySpawnEvent(handle);
        }
    }

    public sealed class EntityTransformEvent : Event, IEvent<EntityTransformEvent>
    {
        private EntityTransformEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventEntityTransform;
            }
        }

        public Entity Entity
        {
            get
            {
                return EventEntity();
            }
        }

        public Entity? Result
        {
            get
            {
                return EventTarget();
            }
        }

        public static EntityTransformEvent Create(nint handle)
        {
            return new EntityTransformEvent(handle);
        }
    }

    public sealed class EntityTargetEvent : Event, IEvent<EntityTargetEvent>
    {
        private EntityTargetEvent(nint handle) : base(handle)
        {
        }

        public static uint EventType
        {
            get
            {
                return FalconConstants.EventEntityTarget;
            }
        }

        public Entity Entity
        {
            get
            {
                return EventEntity();
            }
        }

        public Entity? Target
        {
            get
            {
                return EventTarget();
            }
        }

        public static EntityTargetEvent Create(nint handle)
        {
            return new EntityTargetEvent(handle);
        }
    }
}
