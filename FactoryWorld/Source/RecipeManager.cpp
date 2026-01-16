#include "RecipeManager.h"

void RecipeManager::InitDefaults()
{
    mRecipes.clear();
    // Create the basic recipes, this could also be saved in a JSON and charged at the start with the AddRecipe

    // Iron Ingot: 2 IronOre -> 1 IronIngot
    Recipe r;
    r.id = "iron_ingot";
    r.inputs = { {ItemType::IronOre, 2} };
    r.outputs = { {ItemType::IronIngot, 1} };
    r.craftTimeMs = 2500;
    r.itemSpritePath = "Media/Items/IronIngot.png";
    mRecipes.push_back(r);

    // Copper Ingot: 2 CopperOre -> 1 CopperIngot
    r.id = "copper_ingot";
    r.inputs = { {ItemType::CopperOre, 2} };
    r.outputs = { {ItemType::CopperIngot, 1} };
    r.craftTimeMs = 2500;
    r.itemSpritePath = "Media/Items/CopperIngot.png";
    mRecipes.push_back(r);

    // Iron Rod: 1 IronIngot -> 1 IronRod
    r.id = "iron_rod";
    r.inputs = { {ItemType::IronIngot, 1} };
    r.outputs = { {ItemType::IronRod, 1} };
    r.craftTimeMs = 1000;
    r.itemSpritePath = "Media/Items/IronRod.png";
    mRecipes.push_back(r);

    // Iron Wrench: 1 IronRod -> 1 IronWrench
    r.id = "iron_wrench";
    r.inputs = { {ItemType::IronRod, 1} };
    r.outputs = { {ItemType::IronWrench, 1} };
    r.craftTimeMs = 1500;
    r.itemSpritePath = "Media/Items/IronWrench.png";
    mRecipes.push_back(r);

    // Iron Gear: 1 IronWrench + 1 IronIngot -> 1 IronGear
    r.id = "iron_gear";
    r.inputs = { {ItemType::IronWrench, 1}, {ItemType::IronIngot, 1} };
    r.outputs = { {ItemType::IronGear, 1} };
    r.craftTimeMs = 3000;
    r.itemSpritePath = "Media/Items/IronGear.png";
    mRecipes.push_back(r);

    // Copper Gear: 1 IronWrench + 1 CopperIngot -> 1 CopperGear
    r.id = "copper_gear";
    r.inputs = { {ItemType::IronWrench, 1}, {ItemType::CopperIngot, 1} };
    r.outputs = { {ItemType::CopperGear, 1} };
    r.craftTimeMs = 3000;
    r.itemSpritePath = "Media/Items/CopperGear.png";
    mRecipes.push_back(r);
}

const Recipe* RecipeManager::GetRecipeById(const std::string& id) const
{
    for (const Recipe& r : mRecipes)
        if (r.id == id) return &r;
    return nullptr;
}

const Recipe* RecipeManager::GetNextRecipeById(const std::string& id) const
{
    for (int i = 0; i < mRecipes.size(); i++)
    {
        if (mRecipes[i].id == id)
        {
            // Checks if the recipe is the last one
            if (i == mRecipes.size() - 1)
            {
                return &mRecipes[0];
            }
            return &mRecipes[i + 1];
        }
    }
    return nullptr;
}

bool RecipeManager::AddRecipe(const Recipe& newRecipe)
{
    // Cehck if already exists
    for (const Recipe& r : mRecipes)
        if (r.id == newRecipe.id) return false;

    mRecipes.push_back(newRecipe);
    return true;
}
