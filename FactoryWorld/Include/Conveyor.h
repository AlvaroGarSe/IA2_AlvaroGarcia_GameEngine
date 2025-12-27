#pragma once
#include "SDL.h"
#include "Object.h"

class Conveyor :public Object
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

	//Set position of the conveyor
	void setPosition(int posX, int posY);

	//Loads media
	bool loadMedia();

	Orientation orientation;
};