#pragma once
#include "SDL.h"
#include "Object.h"

class Player :public Object
{
public:

	//Initializes the variables
	Player(int posX = 0, int posY = 0);

	//Set position of the conveyor
	void setPosition(int posX, int posY);

	//Loads media
	bool loadMedia();
};