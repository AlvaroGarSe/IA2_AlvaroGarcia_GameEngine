#pragma once
#include "Singleton.h"

// Manages the controls of the player
class PlayerControlls :public Singleton<PlayerControlls>
{
	friend class Singleton<PlayerControlls>;

public:
	// Create buildings: 1 = Miner || 2 = Conveyor || 3 = Crafter
	void CreateMiner(int cx, int cy);
	void CreateConveyor(int cx, int cy);
	void CreateCrafter(int cx, int cy);

	// Remove buildings: Right Click or Backspace
	void RemoveBuilding(int cx, int cy);

	// Rotate building: <--Q || E-->
	void RotateBuilding(int cx, int cy, int dir);

	// Change recipe of crafter: R
	void ChangeCrafterRecipe(int cx, int cy);
};
