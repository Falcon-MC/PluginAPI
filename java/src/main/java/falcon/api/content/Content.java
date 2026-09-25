package falcon.api.content;

import falcon.api.BlockPos;
import falcon.api.Entity;
import falcon.api.Item;
import falcon.api.Player;
import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.internal.Upcalls;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandles;

public final class Content {
    private static final MethodHandles.Lookup LOOKUP = MethodHandles.lookup();
    private static final MemorySegment USE_ITEM = Upcalls.stub(LOOKUP, "useItem", FalconAbi.ITEM_USE_HANDLER);
    private static final MemorySegment USE_ITEM_ON_BLOCK =
            Upcalls.stub(LOOKUP, "useItemOnBlock", FalconAbi.ITEM_USE_ON_BLOCK_HANDLER);
    private static final MemorySegment INTERACT_BLOCK =
            Upcalls.stub(LOOKUP, "interactBlock", FalconAbi.BLOCK_INTERACT_HANDLER);
    private static final MemorySegment BREAK_BLOCK = Upcalls.stub(LOOKUP, "breakBlock", FalconAbi.BLOCK_BREAK_HANDLER);
    private static final MemorySegment TICK_ENTITY = Upcalls.stub(LOOKUP, "tickEntity", FalconAbi.ENTITY_TICK_HANDLER);
    private static final MemorySegment INTERACT_ENTITY =
            Upcalls.stub(LOOKUP, "interactEntity", FalconAbi.ENTITY_INTERACT_HANDLER);
    private static final String ITEM_FAILURE = "A custom item handler threw an exception";
    private static final String BLOCK_FAILURE = "A custom block handler threw an exception";
    private static final String ENTITY_FAILURE = "A custom entity handler threw an exception";

    private final PluginContext mContext;

    public Content(PluginContext context) {
        mContext = context;
    }

    public boolean item(CustomItem definition) {
        ItemHandlers handlers = new ItemHandlers(mContext, definition.onUse(), definition.onUseOnBlock());
        MemorySegment userData = Upcalls.register(handlers);
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment descriptor = arena.allocate(FalconAbi.CustomItemDescriptor.LAYOUT);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.IDENTIFIER,
                    Interop.text(arena, definition.identifier()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.DISPLAY_NAME,
                    Interop.nullableText(arena, definition.displayName()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.ICON,
                    Interop.nullableText(arena, definition.icon()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.CREATIVE_CATEGORY,
                    Interop.nullableText(arena, definition.creativeCategory()));
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.CustomItemDescriptor.MAX_STACK_SIZE,
                    definition.maxStackSize());
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.CustomItemDescriptor.MAX_DURABILITY,
                    definition.maxDurability());
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.CustomItemDescriptor.HAND_EQUIPPED,
                    definition.handEquipped() ? 1 : 0);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.ON_USE,
                    handlers.onUse() != null ? USE_ITEM : MemorySegment.NULL);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.ON_USE_ON_BLOCK,
                    handlers.onUseOnBlock() != null ? USE_ITEM_ON_BLOCK : MemorySegment.NULL);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomItemDescriptor.USER_DATA, userData);
            return registered(Interop.api().registerCustomItem(mContext.handle(), descriptor), userData);
        }
    }

    public boolean block(CustomBlock definition) {
        BlockHandlers handlers = new BlockHandlers(mContext, definition.onInteract(), definition.onBreak());
        MemorySegment userData = Upcalls.register(handlers);
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment descriptor = arena.allocate(FalconAbi.CustomBlockDescriptor.LAYOUT);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.IDENTIFIER,
                    Interop.text(arena, definition.identifier()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.DISPLAY_NAME,
                    Interop.nullableText(arena, definition.displayName()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.TEXTURE,
                    Interop.nullableText(arena, definition.texture()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.CREATIVE_CATEGORY,
                    Interop.nullableText(arena, definition.creativeCategory()));
            descriptor.set(ValueLayout.JAVA_FLOAT, FalconAbi.CustomBlockDescriptor.DESTROY_TIME,
                    definition.destroyTime());
            descriptor.set(ValueLayout.JAVA_FLOAT, FalconAbi.CustomBlockDescriptor.EXPLOSION_RESISTANCE,
                    definition.explosionResistance());
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.CustomBlockDescriptor.LIGHT_EMISSION,
                    definition.lightEmission());
            descriptor.set(ValueLayout.JAVA_FLOAT, FalconAbi.CustomBlockDescriptor.FRICTION, definition.friction());
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.DROP,
                    Interop.nullableText(arena, definition.drop()));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.ON_INTERACT,
                    handlers.onInteract() != null ? INTERACT_BLOCK : MemorySegment.NULL);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.ON_BREAK,
                    handlers.onBreak() != null ? BREAK_BLOCK : MemorySegment.NULL);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomBlockDescriptor.USER_DATA, userData);
            return registered(Interop.api().registerCustomBlock(mContext.handle(), descriptor), userData);
        }
    }

    public boolean entity(CustomEntity definition) {
        EntityHandlers handlers = new EntityHandlers(mContext, definition.onTick(), definition.onInteract());
        MemorySegment userData = Upcalls.register(handlers);
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment descriptor = arena.allocate(FalconAbi.CustomEntityDescriptor.LAYOUT);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomEntityDescriptor.IDENTIFIER,
                    Interop.text(arena, definition.identifier()));
            descriptor.set(ValueLayout.JAVA_FLOAT, FalconAbi.CustomEntityDescriptor.WIDTH, definition.width());
            descriptor.set(ValueLayout.JAVA_FLOAT, FalconAbi.CustomEntityDescriptor.HEIGHT, definition.height());
            descriptor.set(ValueLayout.JAVA_FLOAT, FalconAbi.CustomEntityDescriptor.MAX_HEALTH,
                    definition.maxHealth());
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.CustomEntityDescriptor.SUMMONABLE,
                    definition.summonable() ? 1 : 0);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomEntityDescriptor.ON_TICK,
                    handlers.onTick() != null ? TICK_ENTITY : MemorySegment.NULL);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomEntityDescriptor.ON_INTERACT,
                    handlers.onInteract() != null ? INTERACT_ENTITY : MemorySegment.NULL);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CustomEntityDescriptor.USER_DATA, userData);
            return registered(Interop.api().registerCustomEntity(mContext.handle(), descriptor), userData);
        }
    }

    private static boolean registered(int result, MemorySegment userData) {
        if (result == 0) {
            Upcalls.release(userData, Object.class);
            return false;
        }
        return true;
    }

    private static int useItem(MemorySegment player, MemorySegment item, MemorySegment userData) {
        try {
            ItemHandlers handlers = Upcalls.target(userData, ItemHandlers.class);
            if (handlers == null || handlers.onUse() == null) {
                return 0;
            }
            PluginContext context = handlers.context();
            Player user = new Player(context, player);
            Item used = Item.borrow(item);
            return context.test(ITEM_FAILURE, () -> handlers.onUse().use(user, used)) ? 1 : 0;
        } catch (Throwable ignored) {
            return 0;
        }
    }

    private static int useItemOnBlock(MemorySegment player, MemorySegment item, MemorySegment position, int face,
                                      MemorySegment userData) {
        try {
            ItemHandlers handlers = Upcalls.target(userData, ItemHandlers.class);
            if (handlers == null || handlers.onUseOnBlock() == null) {
                return 0;
            }
            PluginContext context = handlers.context();
            Player user = new Player(context, player);
            Item used = Item.borrow(item);
            BlockPos target = Interop.blockPos(position);
            return context.test(ITEM_FAILURE, () -> handlers.onUseOnBlock().use(user, used, target, face)) ? 1 : 0;
        } catch (Throwable ignored) {
            return 0;
        }
    }

    private static int interactBlock(MemorySegment player, MemorySegment position, int face,
                                     MemorySegment userData) {
        try {
            BlockHandlers handlers = Upcalls.target(userData, BlockHandlers.class);
            if (handlers == null || handlers.onInteract() == null) {
                return 0;
            }
            PluginContext context = handlers.context();
            Player user = new Player(context, player);
            BlockPos target = Interop.blockPos(position);
            return context.test(BLOCK_FAILURE, () -> handlers.onInteract().interact(user, target, face)) ? 1 : 0;
        } catch (Throwable ignored) {
            return 0;
        }
    }

    private static void breakBlock(MemorySegment player, MemorySegment position, MemorySegment userData) {
        try {
            BlockHandlers handlers = Upcalls.target(userData, BlockHandlers.class);
            if (handlers == null || handlers.onBreak() == null) {
                return;
            }
            PluginContext context = handlers.context();
            Player user = new Player(context, player);
            BlockPos target = Interop.blockPos(position);
            context.guard(BLOCK_FAILURE, () -> handlers.onBreak().broken(user, target));
        } catch (Throwable ignored) {
        }
    }

    private static void tickEntity(MemorySegment entity, MemorySegment userData) {
        try {
            EntityHandlers handlers = Upcalls.target(userData, EntityHandlers.class);
            if (handlers == null || handlers.onTick() == null) {
                return;
            }
            PluginContext context = handlers.context();
            Entity ticked = new Entity(context, entity);
            context.guard(ENTITY_FAILURE, () -> handlers.onTick().tick(ticked));
        } catch (Throwable ignored) {
        }
    }

    private static int interactEntity(MemorySegment entity, MemorySegment player, MemorySegment userData) {
        try {
            EntityHandlers handlers = Upcalls.target(userData, EntityHandlers.class);
            if (handlers == null || handlers.onInteract() == null) {
                return 0;
            }
            PluginContext context = handlers.context();
            Entity target = new Entity(context, entity);
            Player user = new Player(context, player);
            return context.test(ENTITY_FAILURE, () -> handlers.onInteract().interact(target, user)) ? 1 : 0;
        } catch (Throwable ignored) {
            return 0;
        }
    }

    private record ItemHandlers(PluginContext context, ItemUseHandler onUse, ItemUseOnBlockHandler onUseOnBlock) {
    }

    private record BlockHandlers(PluginContext context, BlockInteractHandler onInteract, BlockBreakHandler onBreak) {
    }

    private record EntityHandlers(PluginContext context, EntityTickHandler onTick,
                                  EntityInteractHandler onInteract) {
    }
}
