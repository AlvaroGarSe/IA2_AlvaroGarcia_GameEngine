#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"

class Projectile:public Object
{
public:

	//Initializes the variables
	Projectile(int SHIP_WIDHT, int shipPosX, int shipPosY);

	//Moves the projectile
	void move();

	//Shows the projectile on the screen
	void render();

	//Spawns the projectile in the head of the ship
	void GenerateProjectile(int SHIP_WIDHT, int shipPosX, int shipPosY);

	//Loads media
	bool loadMedia();

	LTexture* getProjectileTexture() { return gProjectileTexture; };

private:
	//The velocity of the projectile
	int mVelY;

	//Scene textures
	LTexture* gProjectileTexture = new LTexture;
};