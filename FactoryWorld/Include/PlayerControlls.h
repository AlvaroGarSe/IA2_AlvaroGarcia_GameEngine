#pragma once
#include "Singleton.h"

class PlayerControlls :public Singleton<PlayerControlls>
{
	friend class Singleton<PlayerControlls>;

public:
	void CreateMiner(int cx, int cy);
	void CreateConveyor(int cx, int cy);
	void CreateCrafter(int cx, int cy);

	void RemoveBuilding(int cx, int cy);

	void RotateBuilding(int cx, int cy, int dir);
	void ChangeCrafterRecipe(int cx, int cy);
};
