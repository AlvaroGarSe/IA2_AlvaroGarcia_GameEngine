#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"
#include <iostream>
using namespace std;

class Background:public Object
{
public:
	// Default path of the media of the background
	string PATH = "../../03-Media/Background.png";

	//Initializes the variables
	Background(int ScreenWidht, int ScreenHeight, string path);

	//Shows the dot on the screen
	void render();

	//Loads media
	bool loadMedia();

	LTexture* getBackgroundTexture() { return gBackgroundTexture; };

private:

	//Scene textures
	LTexture* gBackgroundTexture = new LTexture;
};