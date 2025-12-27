#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"

class Player :public Object
{
public:

	//Initializes the variables
	Player(int posX = 0, int posY = 0);

	//Set position of the conveyor
	void setPosition(int posX, int posY);

	void render();

	//Loads media
	bool loadMedia();

	LTexture* getPlayerTexture() { return gPlayerTexture; };

private:

	//Scene textures
	LTexture* gPlayerTexture = new LTexture;
};