#pragma once
#include "GameObject.h"
#include "ConveyorTypes.h"

enum class TileType : uint8_t
{
	Ground,
	IronVein, // Miner produces iron ore
	CopperVein, // Miner produces copper ore
	Water // Can not be build here
};

struct GridCell
{
	TileType tileType = TileType::Ground;
	GameObject* gameObject = nullptr; // Pointer to the object occupying this cell (nullptr if empty)

	ConveyorId conveyorId = INVALID_CONVEYOR; // Used for AoS and SoA in the conveyors
};