#include "RecipeManager.h"

void RecipeManager::InitDefaults()
{
    mRecipes.clear();

    // Iron Ingot: 2 IronOre -> 1 IronIngot
    Recipe r;
    r.id = "iron_ingot";
    r.inputs = { {ItemType::IronOre, 2} };
    r.outputs = { {ItemType::IronIngot, 1} };
    r.craftTimeMs = 2000;
    mRecipes.push_back(r);

    // Copper Ingot: 2 CopperOre -> 1 CopperIngot
    r.id = "copper_ingot";
    r.inputs = { {ItemType::CopperOre, 2} };
    r.outputs = { {ItemType::CopperIngot, 1} };
    r.craftTimeMs = 2000;
    mRecipes.push_back(r);
}

const Recipe* RecipeManager::GetRecipeById(const std::string& id) const
{
    for (const Recipe& r : mRecipes)
        if (r.id == id) return &r;
    return nullptr;
}

bool RecipeManager::AddRecipe(const Recipe& newRecipe)
{
    // already exists?
    for (const Recipe& r : mRecipes)
        if (r.id == newRecipe.id) return false;

    mRecipes.push_back(newRecipe);
    return true;
}
