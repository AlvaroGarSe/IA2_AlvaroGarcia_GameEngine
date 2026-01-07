#pragma once
#include "GameObject.h"
#include "ConveyorTypes.h"

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

	ConveyorId conveyorId = INVALID_CONVEYOR; // Used for AOS in the conveyors
};