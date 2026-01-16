#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "Item.h"

struct ItemStack
{
    ItemType type;
    int amount;
};

// Stores the information of the recipe to then obtain it in the crafter through the RecipeManager
struct Recipe
{
    std::string id;
    std::vector<ItemStack> inputs;
    std::vector<ItemStack> outputs;
    uint32_t craftTimeMs = 1000;
    std::string itemSpritePath;
};
