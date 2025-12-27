#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"

class Conveyor :public Object
{
public:

	//Initializes the variables
	Conveyor(int posX = 0, int posY = 0);

	//Set position of the conveyor
	void setPosition(int posX, int posY);

	void render();

	//Loads media
	bool loadMedia();

	LTexture* getConveyorTexture() { return gConveyorTexture; };

private:

	//Scene textures
	LTexture* gConveyorTexture = new LTexture;
};