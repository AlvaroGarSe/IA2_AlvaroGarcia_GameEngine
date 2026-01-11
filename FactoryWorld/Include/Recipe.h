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

struct Recipe
{
    std::string id;
    std::vector<ItemStack> inputs;
    std::vector<ItemStack> outputs;
    uint32_t craftTimeMs = 1000;
    std::string itemSpritePath;
};
