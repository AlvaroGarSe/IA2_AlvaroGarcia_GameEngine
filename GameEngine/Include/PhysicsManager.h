#pragma once
#include "SDL.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "Singleton.h"
#include "Object.h"




class PhysicsManager:public Singleton<PhysicsManager>
{
	/*****************************************************************************/
	friend class Singleton<PhysicsManager>;
	/*****************************************************************************/

private:
  
	//Screen dimension constants
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 960;

	// Private constructor to avoid more than one instance
	PhysicsManager() {};

  /*****************************************************************************/

public:

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Check the collision between two objects
	bool checkCollision(Object a, Object b);

	//Gives SCREEN_WIDTH
	int getScreenWidth() { return SCREEN_WIDTH; };

	//Gives SCREEN_HEIGHT
	int getScreenHeight() { return SCREEN_HEIGHT; };
};