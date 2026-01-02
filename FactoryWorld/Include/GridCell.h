#pragma once
#include "GameObject.h"

enum class TileType : uint8_t
{
	Ground,
	IronVein,
	CopperVein,
	Water
};

struct GridCell
{
	TileType tileType = TileType::Ground;
	GameObject* gameObject = nullptr; // Pointer to the object occupying this cell (nullptr if empty)
};