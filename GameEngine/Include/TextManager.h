#pragma once
#include "SDL.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Singleton.h"
#include <SDL_ttf.h>
#include "iostream"
#include <string>
using namespace std;



class TextManager:public Singleton<TextManager>
{
	/*****************************************************************************/
	friend class Singleton<TextManager>;
	/*****************************************************************************/

private:
	// Private constructor to avoid more than one instance
	TextManager() {};

  /*****************************************************************************/

public:

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Starts up SDL and creates window
	bool Init();

	// Loaded the font and returns it to then be used in DrawText()
	TTF_Font* LoadFont(string fontPath, int size);

	// Draws text on the selected position
	bool DrawText(string text, SDL_Color textColor, TTF_Font* font, int quadX, int quadY, SDL_Renderer* gRenderer);

	//Frees media and shuts down SDL
	void CloseFont(TTF_Font* font);

};