#pragma once

#include "falcon/Api.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace falcon {
    /**
     * Crafting table recipes. Players online get the new recipe book at once. A removed vanilla recipe is gone until
     * the server restarts.
     */
    class Recipes {
    public:
        /** Ingredients row by row, width x height of them; an empty string leaves the cell empty. */
        static bool addShaped(const std::string &identifier, uint32_t width, uint32_t height,
                              const std::vector<std::string> &ingredients, const std::string &result,
                              uint32_t count = 1) {
            return add(identifier, true, width, height, ingredients, result, count);
        }

        static bool addShapeless(const std::string &identifier, const std::vector<std::string> &ingredients,
                                 const std::string &result, uint32_t count = 1) {
            return add(identifier, false, 0, 0, ingredients, result, count);
        }

        /** Removes a recipe added by a plugin or a pack, or a vanilla crafting recipe by its identifier. */
        static bool remove(const std::string &identifier) {
            return detail::api().removeRecipe(identifier.c_str()) != 0;
        }

    private:
        static bool add(const std::string &identifier, bool shaped, uint32_t width, uint32_t height,
                        const std::vector<std::string> &ingredients, const std::string &result, uint32_t count) {
            std::vector<const char *> items;
            items.reserve(ingredients.size());
            for (const std::string &ingredient: ingredients)
                items.push_back(ingredient.c_str());

            const FalconRecipeDescriptor descriptor{identifier.c_str(), shaped ? 1 : 0, width, height, items.data(),
                                                    static_cast<uint32_t>(items.size()), result.c_str(), count};
            return detail::api().registerRecipe(detail::plugin(), &descriptor) != 0;
        }
    };
}
