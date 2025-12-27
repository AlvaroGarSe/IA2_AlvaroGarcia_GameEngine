#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>
#include "LTexture.h"
#include "Conveyor.h"
#include "GraphicManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "Miner.h"
#include "Player.h"

#include "iostream"
using namespace std;

int main(int argc, char* args[])
{
	// Create all the managers
	InputManager::CreateSingleton();
	GraphicManager::CreateSingleton();
	ObjectManager::CreateSingleton();
	TimeManager::CreateSingleton();


	GraphicManager::GetInstance().setScreenSize(1280, 960);

	int SCREEN_WIDTH = GraphicManager::GetInstance().getScreenWidth();
	int SCREEN_HEIGHT = GraphicManager::GetInstance().getScreenHeight();

	Miner miner1 = Miner(50, 50);

	Conveyor conveyor1 = Conveyor(30, 30);
	
	int i = 0;

	//Start up SDL and create window
	if (!GraphicManager::GetInstance().init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!miner1.loadMedia())
		{
			printf("Failed to load media!\n");
		}
		if (!conveyor1.loadMedia())
		{
			printf("Failed to load media!\n");
		}

		miner1.isActive = true;
		conveyor1.isActive = true;


		bool quit = false;


		while (!quit)
		{
			InputManager::GetInstance().Update();

			SDL_Event& e = *InputManager::GetInstance().GetSDLEvent();

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{	
					quit = true;
				}
			}

			// User can stop the game by pressing Escape
			if (InputManager::GetInstance().GetKey(SDL_SCANCODE_ESCAPE))
			{
				quit = true;
			}

			miner1.render();
			conveyor1.render();	


			SDL_RenderPresent(GraphicManager::GetInstance().gRenderer);
		}

	}
	GraphicManager::GetInstance().close();
	miner1.texture->free();
	conveyor1.texture->free();

	return 0;

}