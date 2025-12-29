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
#include "GridManager.h"
#include "Miner.h"
#include "Player.h"
#include "Crafter.h"

#include "iostream"
using namespace std;

int main(int argc, char* args[])
{
	// Create all the managers
	InputManager::CreateSingleton();
	GraphicManager::CreateSingleton();
	ObjectManager::CreateSingleton();
	TimeManager::CreateSingleton();
	GridManager::CreateSingleton();


	GraphicManager::GetInstance().setScreenSize(1280, 960);

	int SCREEN_WIDTH = GraphicManager::GetInstance().getScreenWidth();
	int SCREEN_HEIGHT = GraphicManager::GetInstance().getScreenHeight();


	GridManager::GetInstance().CreateGrid(50, 50, 64);

	ObjectManager objManager = ObjectManager::GetInstance();

	ObjectManager::GetInstance().AddObject(new Miner(GameObject::Orientation::EAST));

	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::SOUTH));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));
	ObjectManager::GetInstance().AddObject(new Conveyor(GameObject::Orientation::EAST));


	ObjectManager::GetInstance().AddObject(new Crafter(GameObject::Orientation::EAST));

	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(0), 1, 1);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(1), 2, 1);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(2), 3, 1);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(3), 4, 1);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(4), 5, 1);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(5), 5, 2);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(6), 6, 2);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(7), 8, 2);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(8), 9, 2);
	GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(9), 7, 2);

	/*Miner miner1 = Miner(GameObject::Orientation::EAST);

	Conveyor conveyor1 = Conveyor(GameObject::Orientation::EAST);
	Conveyor conveyor2 = Conveyor(GameObject::Orientation::EAST);
	Conveyor conveyor3 = Conveyor(GameObject::Orientation::EAST);
	Conveyor conveyor4 = Conveyor(GameObject::Orientation::SOUTH);
	Conveyor conveyor5 = Conveyor(GameObject::Orientation::EAST);
	Conveyor conveyor6 = Conveyor(GameObject::Orientation::EAST);
	Conveyor conveyor7 = Conveyor(GameObject::Orientation::EAST);
	Conveyor conveyor8 = Conveyor(GameObject::Orientation::EAST);

	Crafter crafter1 = Crafter(GameObject::Orientation::EAST);

	GridManager::GetInstance().PlaceObject(&miner1, 1, 1);
	GridManager::GetInstance().PlaceObject(&conveyor1, 2, 1);
	GridManager::GetInstance().PlaceObject(&conveyor2, 3, 1);
	GridManager::GetInstance().PlaceObject(&conveyor3, 4, 1);
	GridManager::GetInstance().PlaceObject(&conveyor4, 5, 1);
	GridManager::GetInstance().PlaceObject(&conveyor5, 5, 2);
	GridManager::GetInstance().PlaceObject(&conveyor6, 6, 2);
	GridManager::GetInstance().PlaceObject(&conveyor7, 8, 2);
	GridManager::GetInstance().PlaceObject(&conveyor8, 9, 2);

	GridManager::GetInstance().PlaceObject(&crafter1, 7, 2);*/


	
	int i = 0;

	//Start up SDL and create window
	if (!GraphicManager::GetInstance().init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		TimeManager::GetInstance().start();



		//if (!miner1.LoadMedia())
		//{
		//	printf("Failed to load media!\n");
		//}
		//if (!conveyor1.LoadMedia())
		//{
		//	printf("Failed to load media!\n");
		//}
		//conveyor2.LoadMedia();
		//conveyor3.LoadMedia();
		//conveyor4.LoadMedia();
		//conveyor5.LoadMedia();
		//conveyor6.LoadMedia();
		//conveyor7.LoadMedia();
		//conveyor8.LoadMedia();

		//crafter1.LoadMedia();

		ObjectManager::GetInstance().LoadAllMedia();


		ObjectManager::GetInstance().Start();



		//miner1.StartMiner();

		//crafter1.StartCrafter();


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

			ObjectManager::GetInstance().Update();

		/*	miner1.Update();
			conveyor1.Update();
			conveyor2.Update();
			conveyor3.Update();
			conveyor4.Update();
			conveyor5.Update();
			conveyor6.Update();
			conveyor7.Update();
			conveyor8.Update();

			crafter1.Update();*/


			GraphicManager::GetInstance().BeginFrame();
			GridManager::GetInstance().RenderDebugGrid();

			ObjectManager::GetInstance().RenderAllObjects();

			//miner1.Render();
			//conveyor1.Render();
			//conveyor2.Render();
			//conveyor3.Render();
			//conveyor4.Render();
			//conveyor5.Render();
			//conveyor6.Render();
			//conveyor7.Render();
			//conveyor8.Render();

			//crafter1.Render();

			GraphicManager::GetInstance().EndFrame();
		}

	}
	GraphicManager::GetInstance().close();
	/*miner1.texture->free();
	conveyor1.texture->free();*/

	ObjectManager::GetInstance().FreeObjects();

	return 0;

}