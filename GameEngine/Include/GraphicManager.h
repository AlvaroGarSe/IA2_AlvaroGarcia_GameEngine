#pragma once
#include "SDL.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Singleton.h"



class GraphicManager:public Singleton<GraphicManager>
{
	/*****************************************************************************/
	friend class Singleton<GraphicManager>;
	/*****************************************************************************/

private:
  
	//Screen basic dimension (not constants)
	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 960;

	// Private constructor to avoid more than one instance
	GraphicManager() {};

  /*****************************************************************************/

public:

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Starts up SDL and creates window
	bool init();

	//Frees media and shuts down SDL
	void close();

	void setScreenSize(int screenWidht, int screenHeight) { SCREEN_WIDTH = screenWidht;  SCREEN_HEIGHT = screenHeight; };

	//Gives SCREEN_WIDTH
	int getScreenWidth() { return SCREEN_WIDTH; };

	//Gives SCREEN_HEIGHT
	int getScreenHeight() { return SCREEN_HEIGHT; };
};