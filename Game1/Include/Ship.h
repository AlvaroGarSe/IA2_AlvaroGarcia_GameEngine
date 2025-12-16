#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "InputManager.h"
#include "Object.h"

class Ship:public Object
{
public:

	int SCREEN_WIDTH;

	//Maximum axis velocity of the dot
	static const int SHIP_VEL = 5;

	//Initializes the variables
	Ship(int screenWidth, int screenHeight);

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(InputManager& inpManager);

	//Moves the ship
	void move();

	//Shows the ship on the screen
	void render();

	//Loads media
	bool loadMedia();

	LTexture* getShipTexture() { return gShipTexture; };

private:

	//The velocity of the ship
	float mVelX;

	//Scene textures
	LTexture* gShipTexture = new LTexture;
};