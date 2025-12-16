#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"

class Pipe:public Object
{
public:
	int SCREEN_WIDTH;
	int SCREEN_HEIGTH;

	// Activated if passig the pipe has alread given score
	bool hasGivenScore = false;
	
	// Activated if the pipe is generated in the top of the screen
	bool isTop = false;

	//Maximum axis velocity of the pipe
	static const int PIPE_VEL = 2;

	//Initializes the variables
	Pipe(int screenWidth, int screenHeight);

	//Moves the pipe
	void move();

	// Return true if the bird has passed one time through the pipe and false if not or if already passed
	bool checkScore(int posXBird);

	//Shows the pipe on the screen
	void render();

	//Loads media
	bool loadMedia();

	void GeneratePipe(bool up, int shift);

	LTexture* getPipeTexture() { return gPipeTexture; };

private:

	//The velocity of the pipe
	float mVelX;

	//Scene textures
	LTexture* gPipeTexture = new LTexture;
};