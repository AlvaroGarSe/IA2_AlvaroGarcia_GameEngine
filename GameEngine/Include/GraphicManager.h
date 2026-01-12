#pragma once
#include "SDL.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Singleton.h"
#include "LTexture.h"
#include "Transform.h"
#include "Camera2D.h"



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
	SDL_Window* gWindow = nullptr;

	//The window renderer
	SDL_Renderer* gRenderer = nullptr;

	Camera2D camera;

	//Starts up SDL and creates window
	bool init();

	//Frees media and shuts down SDL
	void close();

	// Prepare rendering
	void BeginFrame();

	// Present frame
	void EndFrame();

	void setScreenSize(int screenWidth, int screenHeight);

	//Gives SCREEN_WIDTH
	int getScreenWidth() { return SCREEN_WIDTH; };

	//Gives SCREEN_HEIGHT
	int getScreenHeight() { return SCREEN_HEIGHT; };

	// FULLSCREEN
	bool SetFullscreen(bool enabled, bool borderless = true);
	void ToggleFullscreen(bool borderless = true);
	bool IsFullscreen() const;

	void DrawTexture(LTexture* texture, const Transform& transform, SDL_Rect* clip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DrawTextureSoA(LTexture* texture, int x, int y, double rotation, float scaleX, float scaleY, SDL_Rect* clip = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// Updates SCREEN_WIDTH/SCREEN_HEIGHT from SDL
	void RefreshWindowSizeCache();

};