package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.NativeApi;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.util.List;

/**
 * Crafting table recipes. Players online get the new recipe book at once. A removed vanilla recipe is gone until the
 * server restarts.
 */
public final class Recipes {
    private final PluginContext mContext;

    Recipes(PluginContext context) {
        mContext = context;
    }

    /**
     * Ingredients row by row, width x height of them; an empty string leaves the cell empty.
     */
    public boolean addShaped(String identifier, int width, int height, List<String> ingredients, String result,
                             int count) {
        return add(identifier, true, width, height, ingredients, result, count);
    }

    public boolean addShapeless(String identifier, List<String> ingredients, String result, int count) {
        return add(identifier, false, 0, 0, ingredients, result, count);
    }

    /**
     * Removes a recipe added by a plugin or a pack, or a vanilla crafting recipe by its identifier.
     */
    public boolean remove(String identifier) {
        try (Arena arena = Arena.ofConfined()) {
            return api().removeRecipe(Interop.text(arena, identifier)) != 0;
        }
    }

    private boolean add(String identifier, boolean shaped, int width, int height, List<String> ingredients,
                        String result, int count) {
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment items = arena.allocate(ValueLayout.ADDRESS, Math.max(ingredients.size(), 1));
            for (int index = 0; index < ingredients.size(); index++) {
                items.setAtIndex(ValueLayout.ADDRESS, index, Interop.text(arena, ingredients.get(index)));
            }

            MemorySegment descriptor = arena.allocate(FalconAbi.RecipeDescriptor.LAYOUT);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.RecipeDescriptor.IDENTIFIER, Interop.text(arena, identifier));
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.RecipeDescriptor.SHAPED, shaped ? 1 : 0);
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.RecipeDescriptor.WIDTH, width);
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.RecipeDescriptor.HEIGHT, height);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.RecipeDescriptor.INGREDIENTS, items);
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.RecipeDescriptor.INGREDIENT_COUNT, ingredients.size());
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.RecipeDescriptor.RESULT, Interop.text(arena, result));
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.RecipeDescriptor.RESULT_COUNT, count);
            return api().registerRecipe(mContext.handle(), descriptor) != 0;
        }
    }

    private static NativeApi api() {
        return Interop.api();
    }
}
