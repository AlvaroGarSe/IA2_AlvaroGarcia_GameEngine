#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"

class Asteroid:public Object
{
public:	

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	//Initializes the variables
	Asteroid(int screenWidth, int screenHeigth);

	//Moves the Asteroid
	void move();

	//Shows the Asteroid on the screen
	void render();

	//Shows the Asteroid on the screen
	void GenerateAsteroid();

	//Loads media
	bool loadMedia();

	LTexture* getAsteroidTexture() { return gAsteroidTexture; };

private:
	// Velocity of the Asteroid
	int mVelY;

	//Scene textures
	LTexture* gAsteroidTexture = new LTexture;
};