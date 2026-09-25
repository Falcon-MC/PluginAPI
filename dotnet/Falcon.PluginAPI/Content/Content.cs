using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Falcon.Interop;

namespace Falcon
{
    public sealed unsafe class Content
    {
        private const string ItemFailure = "A custom item handler threw an exception";
        private const string BlockFailure = "A custom block handler threw an exception";
        private const string EntityFailure = "A custom entity handler threw an exception";

        private readonly nint _plugin;

        internal Content(nint plugin)
        {
            _plugin = plugin;
        }

        public bool AddItem(CustomItem definition)
        {
            ArgumentNullException.ThrowIfNull(definition);
            void* userData = PluginCallback.Keep(_plugin, definition);
            bool registered;
            fixed (byte* identifier = NativeApi.Utf8(definition.Identifier))
            fixed (byte* displayName = NativeApi.Nullable(definition.DisplayName))
            fixed (byte* icon = NativeApi.Nullable(definition.Icon))
            fixed (byte* creativeCategory = NativeApi.Nullable(definition.CreativeCategory))
            {
                FalconCustomItemDescriptor descriptor = default;
                descriptor.identifier = identifier;
                descriptor.displayName = displayName;
                descriptor.icon = icon;
                descriptor.creativeCategory = creativeCategory;
                descriptor.maxStackSize = definition.MaxStackSize;
                descriptor.maxDurability = definition.MaxDurability;
                descriptor.handEquipped = definition.HandEquipped ? 1 : 0;
                if (definition.OnUse != null)
                {
                    descriptor.onUse = &UseItem;
                }

                if (definition.OnUseOnBlock != null)
                {
                    descriptor.onUseOnBlock = &UseItemOnBlock;
                }

                descriptor.userData = userData;
                registered = NativeApi.Table->registerCustomItem((FalconPlugin*)_plugin, &descriptor) != 0;
            }

            return Settle(registered, userData);
        }

        public bool AddBlock(CustomBlock definition)
        {
            ArgumentNullException.ThrowIfNull(definition);
            void* userData = PluginCallback.Keep(_plugin, definition);
            bool registered;
            fixed (byte* identifier = NativeApi.Utf8(definition.Identifier))
            fixed (byte* displayName = NativeApi.Nullable(definition.DisplayName))
            fixed (byte* texture = NativeApi.Nullable(definition.Texture))
            fixed (byte* creativeCategory = NativeApi.Nullable(definition.CreativeCategory))
            fixed (byte* drop = NativeApi.Nullable(definition.Drop))
            {
                FalconCustomBlockDescriptor descriptor = default;
                descriptor.identifier = identifier;
                descriptor.displayName = displayName;
                descriptor.texture = texture;
                descriptor.creativeCategory = creativeCategory;
                descriptor.destroyTime = definition.DestroyTime;
                descriptor.explosionResistance = definition.ExplosionResistance;
                descriptor.lightEmission = definition.LightEmission;
                descriptor.friction = definition.Friction;
                descriptor.drop = drop;
                if (definition.OnInteract != null)
                {
                    descriptor.onInteract = &InteractBlock;
                }

                if (definition.OnBreak != null)
                {
                    descriptor.onBreak = &BreakBlock;
                }

                descriptor.userData = userData;
                registered = NativeApi.Table->registerCustomBlock((FalconPlugin*)_plugin, &descriptor) != 0;
            }

            return Settle(registered, userData);
        }

        public bool AddEntity(CustomEntity definition)
        {
            ArgumentNullException.ThrowIfNull(definition);
            void* userData = PluginCallback.Keep(_plugin, definition);
            bool registered;
            fixed (byte* identifier = NativeApi.Utf8(definition.Identifier))
            {
                FalconCustomEntityDescriptor descriptor = default;
                descriptor.identifier = identifier;
                descriptor.width = definition.Width;
                descriptor.height = definition.Height;
                descriptor.maxHealth = definition.MaxHealth;
                descriptor.summonable = definition.Summonable ? 1 : 0;
                if (definition.OnTick != null)
                {
                    descriptor.onTick = &TickEntity;
                }

                if (definition.OnInteract != null)
                {
                    descriptor.onInteract = &InteractEntity;
                }

                descriptor.userData = userData;
                registered = NativeApi.Table->registerCustomEntity((FalconPlugin*)_plugin, &descriptor) != 0;
            }

            return Settle(registered, userData);
        }

        private static bool Settle(bool registered, void* userData)
        {
            if (!registered)
            {
                PluginCallback.Release(userData);
            }

            return registered;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static int UseItem(FalconPlayer* player, FalconItem* item, void* userData)
        {
            bool used = PluginCallback.Run(userData, false, ItemFailure, ((nint)player, (nint)item),
                                           static (target, state) =>
                                           {
                                               Player user = new Player((FalconPlayer*)state.Item1);
                                               Item held = Item.Borrow((FalconItem*)state.Item2);
                                               return ((CustomItem)target).OnUse!(user, held);
                                           }, false);
            return used ? 1 : 0;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static int UseItemOnBlock(FalconPlayer* player, FalconItem* item, FalconBlockPos position, uint face,
                                          void* userData)
        {
            bool used = PluginCallback.Run(userData, false, ItemFailure, ((nint)player, (nint)item, position, face),
                                           static (target, state) =>
                                           {
                                               Player user = new Player((FalconPlayer*)state.Item1);
                                               Item held = Item.Borrow((FalconItem*)state.Item2);
                                               BlockPos block = BlockPos.From(state.Item3);
                                               CustomItem definition = (CustomItem)target;
                                               return definition.OnUseOnBlock!(user, held, block, state.Item4);
                                           }, false);
            return used ? 1 : 0;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static int InteractBlock(FalconPlayer* player, FalconBlockPos position, uint face, void* userData)
        {
            bool handled = PluginCallback.Run(userData, false, BlockFailure, ((nint)player, position, face),
                                              static (target, state) =>
                                              {
                                                  Player user = new Player((FalconPlayer*)state.Item1);
                                                  BlockPos block = BlockPos.From(state.Item2);
                                                  return ((CustomBlock)target).OnInteract!(user, block, state.Item3);
                                              }, false);
            return handled ? 1 : 0;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void BreakBlock(FalconPlayer* player, FalconBlockPos position, void* userData)
        {
            PluginCallback.Run(userData, false, BlockFailure, ((nint)player, position), static (target, state) =>
            {
                Player user = new Player((FalconPlayer*)state.Item1);
                ((CustomBlock)target).OnBreak!(user, BlockPos.From(state.Item2));
                return true;
            }, false);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static void TickEntity(FalconEntity* entity, void* userData)
        {
            PluginCallback.Run(userData, false, EntityFailure, (nint)entity, static (target, state) =>
            {
                ((CustomEntity)target).OnTick!(new Entity((FalconEntity*)state));
                return true;
            }, false);
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
        private static int InteractEntity(FalconEntity* entity, FalconPlayer* player, void* userData)
        {
            bool handled = PluginCallback.Run(userData, false, EntityFailure, ((nint)entity, (nint)player),
                                              static (target, state) =>
                                              {
                                                  Entity clicked = new Entity((FalconEntity*)state.Item1);
                                                  Player user = new Player((FalconPlayer*)state.Item2);
                                                  return ((CustomEntity)target).OnInteract!(clicked, user);
                                              }, false);
            return handled ? 1 : 0;
        }
    }
}
