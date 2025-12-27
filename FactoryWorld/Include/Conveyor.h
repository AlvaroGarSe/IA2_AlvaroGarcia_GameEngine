#pragma once
#include "SDL.h"
#include "GameObject.h"

class Conveyor :public GameObject
{
public:

	enum Orientation
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	//Initializes the variables
	Conveyor(int posX = 0, int posY = 0, Orientation orientation = NORTH);

	//Loads media
	bool loadMedia();

	Orientation orientation;
};