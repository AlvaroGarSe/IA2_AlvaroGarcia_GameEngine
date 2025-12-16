#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "InputManager.h"
#include "Object.h"

class Bird:public Object
{
public:
	int SCREEN_HEIGHT;

	// Velocity of the bird when it goes up
	static const int BIRD_VELY = -5;
	// Velocity of the bird when it goes down
	static const int GRAVITY = 2;

	//Initializes the variables
	Bird(int screenWidth, int screenHeight);

	//Takes key presses and adjusts the Bird's velocity
	void handleEvent(InputManager& inpManager);

	//Moves the bird
	void move();

	//Shows the bird on the screen
	void render();

	//Loads media
	bool loadMedia();

	LTexture* getBirdTexture() { return gBirdTexture; };

private:

	//The velocity of the bird
	float mVelY;

	//Scene textures
	LTexture* gBirdTexture = new LTexture;
};