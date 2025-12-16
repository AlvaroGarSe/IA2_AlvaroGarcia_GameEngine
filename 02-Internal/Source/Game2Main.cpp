/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>
#include "LTexture.h"
#include "GraphicManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "Background.h"
#include "PhysicsManager.h"
#include "Bird.h"
#include "Pipe.h"
#include "TextManager.h"

#include "iostream"
#include <random>
using namespace std;

int main( int argc, char* args[] )
{
	// Create all the managers
	InputManager::CreateSingleton();
	GraphicManager::CreateSingleton();
	ObjectManager::CreateSingleton();
	TimeManager::CreateSingleton();
	PhysicsManager::CreateSingleton();
	TextManager::CreateSingleton();

	int SCREEN_WIDTH = GraphicManager::GetInstance().getScreenWidth();
	int SCREEN_HEIGHT = GraphicManager::GetInstance().getScreenHeight();
	
	// Create the background adn the bird
	Background background = Background(SCREEN_WIDTH, SCREEN_HEIGHT, "../../03-Media/bgSkye.jpg");
	Bird bird = Bird(GraphicManager::GetInstance().getScreenWidth(), GraphicManager::GetInstance().getScreenHeight());

	// Create the pool for the pipes
	const int NUMBER_MAX_PIPES = 10;
	Pipe* pipes[NUMBER_MAX_PIPES];
	bool createPipe = false;

	// Used for the spawn rate of the pipes
	const Uint32 pipeDelayMs = 2500; // In milliseconds
	Uint32 lastTimePipe = pipeDelayMs;
	Uint32 Time = 0;

	int score = 0;

	TextManager::GetInstance().init();
	string fontPath = "../../03-Media/Font.ttf";
	TTF_Font* font = TextManager::GetInstance().selectFont(fontPath, 24);
	
	//Start up SDL and create window
	if(!GraphicManager::GetInstance().init())
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		// Used for the generation of the pipes with a random shift
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(-250, 250);
		int shift = 0;
		
		// Load the media of all the objects
		background.loadMedia();
		// Create and add the pipes in the pool and loads the media
		for (int i = 0; i < NUMBER_MAX_PIPES; i++)
		{
			pipes[i] = new Pipe(GraphicManager::GetInstance().getScreenWidth(), GraphicManager::GetInstance().getScreenHeight());
			pipes[i]->loadMedia();
		}
		if( !bird.loadMedia())
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//While application is running
			while( !quit )
			{
				InputManager::GetInstance().Update();
				Time = TimeManager::GetInstance().getTicks();

				//Handle events on queue
				while( SDL_PollEvent(InputManager::GetInstance().GetSDLEvent()) != 0)
				{
					//User requests quit
					if(InputManager::GetInstance().GetSDLEvent()->type == SDL_QUIT )
					{
						quit = true;
					}

					// User can stop the game by pressing Escape
					if (InputManager::GetInstance().GetKey(SDL_SCANCODE_ESCAPE))
					{
						quit = true;
					}

					//Handle input for the bird
					bird.handleEvent(InputManager::GetInstance());
				}
				
				// Generation of the pipe
				createPipe = true;
				for (int i = 0; i < NUMBER_MAX_PIPES - 1 && createPipe && Time - lastTimePipe > pipeDelayMs; i++)
				{
					if (!pipes[i]->isActive)
					{
						// Create a random number thar will be the shift of the pipes
						shift = distr(gen);
						pipes[i]->GeneratePipe(false, shift);
						pipes[i + 1]->GeneratePipe(true, shift);

						createPipe = false;
						lastTimePipe = Time;
					}
				}

				// Move all the objects
				bird.move();
				for (int i = 0; i < NUMBER_MAX_PIPES; i++)
				{
					if (pipes[i]->isActive)
					{
						// Moves all the pipes that are active
						pipes[i]->move();

						// Check if the bird passed the pipe and if so adds score
						if (pipes[i]->checkScore(bird.mPosX)) { score++; }

						// Check if the bird collides with the pipes if so the player lose
						if (PhysicsManager::GetInstance().checkCollision(*pipes[i], bird))
						{
							quit = true;
						}
					}
				}
				
				// Render all the objects in the screen
				background.render();
				bird.render();
				for (int i = 0; i < NUMBER_MAX_PIPES; i++)
				{
					pipes[i]->render();
				}
				
				// Print the score
				SDL_Color textColor = { 0, 0, 0, 0 };
				TextManager::GetInstance().renderFont(textColor, font, 20, 20, score, GraphicManager::GetInstance().gRenderer);

				SDL_RenderPresent(GraphicManager::GetInstance().gRenderer);
			}
		}
	}

	//Free all the objects and close SDL
	GraphicManager::GetInstance().close();
	bird.getBirdTexture()->free();
	for (int i = 0; i < NUMBER_MAX_PIPES; i++)
	{
		pipes[i]->getPipeTexture()->free();
	}
	TextManager::GetInstance().closeFont(font);
	return 0;
}