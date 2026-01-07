#pragma once
#include <cstdint>

enum class ItemType : uint8_t
{
    NONE = 0,
    IronOre,
	IronIngot,
    CopperOre,
    CopperIngot,
    COUNT
};

static constexpr int ITEMTYPE_COUNT = (int)ItemType::COUNT;

struct Item
{
    ItemType type = ItemType::IronOre;  
};
