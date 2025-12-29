#pragma once
#include <cstdint>

enum class ItemType : uint8_t
{
    IronOre,
	IronIngot,
};

struct Item
{
    ItemType type = ItemType::IronOre;  
};
