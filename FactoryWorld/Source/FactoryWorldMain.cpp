#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>
#include "LTexture.h"
#include "Conveyor.h"
#include "GraphicManager.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "GridManager.h"
#include "TextManager.h"
#include "Miner.h"
#include "Player.h"
#include "Crafter.h"
#include "MapLoader.h"
#include "RecipeManager.h"
#include "ConveyorManager.h"

#include "iostream"
using namespace std;

int main(int argc, char* args[])
{
	// Create all the managers
	InputManager::CreateSingleton();
	GraphicManager::CreateSingleton();
	AssetManager::CreateSingleton();
	ObjectManager::CreateSingleton();
	TimeManager::CreateSingleton();
	GridManager::CreateSingleton();
	TextManager::CreateSingleton();
	RecipeManager::CreateSingleton();
	ConveyorManager::CreateSingleton();
	

	if (!GraphicManager::GetInstance().init())
	{
		printf("Failed to initialize!\n");
		return 0;
	}

	GraphicManager::GetInstance().setScreenSize(1280, 960);

	int SCREEN_WIDTH = GraphicManager::GetInstance().getScreenWidth();
	int SCREEN_HEIGHT = GraphicManager::GetInstance().getScreenHeight();

	TextManager::GetInstance().Init();
	RecipeManager::GetInstance().InitDefaults();

	TTF_Font* uiFont = TextManager::GetInstance().LoadFont("Media/Fonts/Ethnocentric-Regular.otf", 20);
	SDL_Color white{ 255,255,255,255 };

	if (!MapLoader::Load("Media/Maps/map01.json"))
	{
		GridManager::GetInstance().CreateGrid(50, 50, 64);
		GridManager::GetInstance().SetTile(26, 26, TileType::IronVein);
	}

	//GridManager::GetInstance().CreateGrid(50, 50, 64);

	int worldW = GridManager::GetInstance().GetWidth() * GridManager::GetInstance().GetCellSize();
	int worldH = GridManager::GetInstance().GetHeight() * GridManager::GetInstance().GetCellSize();
	int screenW = GraphicManager::GetInstance().getScreenWidth();
	int screenH = GraphicManager::GetInstance().getScreenHeight();


	GraphicManager::GetInstance().camera.SetWorldBounds(worldW, worldH, screenW, screenH);

	// centrar (como antes)
	GraphicManager::GetInstance().camera.SetPosition(
		(worldW - screenW) * 0.5f,
		(worldH - screenH) * 0.5f
	);

	ConveyorManager::GetInstance().Init(ConveyorMode::AoS);

	/*ConveyorId id = ConveyorManager::GetInstance().Create(...);
	ConveyorManager::GetInstance().UpdateAll(now);
	ConveyorManager::GetInstance().RenderAll();*/

	/*auto* c1 = ObjectManager::GetInstance().Spawn<Miner>(GameObject::Orientation::EAST);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);
	auto* c1 = ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);
	auto* c1 = ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);
	auto* c1 = ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);
	auto* c1 = ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::SOUTH);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);
	auto* c1 = ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);
	auto* c1 = ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	GridManager::GetInstance().PlaceObject(c1, 5, 5);*/

	//ObjectManager::GetInstance().Spawn<Miner>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::SOUTH);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST);
	//ObjectManager::GetInstance().Spawn<Crafter>(GameObject::Orientation::EAST);

	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Miner>(GameObject::Orientation::EAST), 1, 1);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 2, 1);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 3, 1);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 4, 1);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::SOUTH), 5, 1);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 5, 2);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 6, 2);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 8, 2);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Conveyor>(GameObject::Orientation::EAST), 9, 2);
	//GridManager::GetInstance().PlaceObjectCentered(ObjectManager::GetInstance().Spawn<Crafter>(GameObject::Orientation::EAST), 7, 2);

	ConveyorManager::GetInstance().Create(20, 20, GameObject::Orientation::EAST);

	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(0), 1, 1);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(1), 2, 1);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(2), 3, 1);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(3), 4, 1);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(4), 5, 1);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(5), 5, 2);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(6), 6, 2);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(7), 8, 2);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(8), 9, 2);
	//GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().GetObject(9), 7, 2);

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



	float camSpeed = 400.f; // pixels per second
	

	
	{
		TimeManager::GetInstance().start();

		ObjectManager::GetInstance().LoadAllMedia();


		ObjectManager::GetInstance().StartAll();

		ConveyorManager::GetInstance().StartAll();


		bool quit = false;

		float move = 0.f;


		int mouseX, mouseY;

		// Used to display the FPS
		int msDisplay = 16;
		double fpsTimer = 0.0;
		int fpsFrames = 0;
		int fpsDisplay = 0;


		while (!quit)
		{
			InputManager::GetInstance().Update();

			Uint32 dt = TimeManager::GetInstance().TickFrame();

			if (dt == 0) dt = 1;

			msDisplay = (int)dt;

			// Calculate the FPS
			fpsTimer += (double)dt;   // milliseconds
			fpsFrames++;

			if (fpsTimer >= 500.0) // 500 ms
			{
				fpsDisplay = (int)((fpsFrames * 1000.0) / fpsTimer); // frames/sec
				fpsTimer = 0.0;
				fpsFrames = 0;
			}


			move = camSpeed * (dt / 1000.f);

			SDL_GetMouseState(&mouseX, &mouseY);

			SDL_Event& e = *InputManager::GetInstance().GetSDLEvent();

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{	
					quit = true;
				}

				if (e.type == SDL_MOUSEWHEEL)
				{
					if (e.wheel.y > 0)
					{
						GraphicManager::GetInstance().camera.ZoomAtScreenPoint(1.1, mouseX, mouseY);
					}
					else if (e.wheel.y < 0)
					{
						GraphicManager::GetInstance().camera.ZoomAtScreenPoint(0.9, mouseX, mouseY);
					}
				}
			}

			// User can stop the game by pressing Escape
			if (InputManager::GetInstance().GetKey(SDL_SCANCODE_ESCAPE))
			{
				quit = true;
			}

			// Input to move the camera
			if (InputManager::GetInstance().GetKey(SDL_SCANCODE_A))
				GraphicManager::GetInstance().camera.Move(-move, 0);
			if (InputManager::GetInstance().GetKey(SDL_SCANCODE_D))
				GraphicManager::GetInstance().camera.Move(move, 0);
			if (InputManager::GetInstance().GetKey(SDL_SCANCODE_W))
				GraphicManager::GetInstance().camera.Move(0, -move);
			if (InputManager::GetInstance().GetKey(SDL_SCANCODE_S))
				GraphicManager::GetInstance().camera.Move(0, move);

			// UPDATE ***************************************************************

			// Update all the items
			ObjectManager::GetInstance().UpdateAll();
			ConveyorManager::GetInstance().UpdateAll(TimeManager::GetInstance().DeltaMs());


			// Used to then render the UI
			SDL_Point world = GraphicManager::GetInstance().camera.ScreenToWorld(mouseX, mouseY);
			SDL_Point gridIdx = GridManager::GetInstance().WorldToGrid(world.x, world.y);
			SDL_Point gridCentered = GridManager::GetInstance().IndexToCentered(gridIdx.x, gridIdx.y);

			std::string tileName = "Out of bounds";
			if (GridManager::GetInstance().IsInside(gridIdx.x, gridIdx.y))
			{
				TileType t = GridManager::GetInstance().GetTileWithCellPos(gridIdx.x, gridIdx.y);
				switch (t)
				{
				case TileType::Ground: tileName = "Ground"; break;
				case TileType::IronVein: tileName = "IronVein"; break;
				case TileType::CopperVein: tileName = "CopperVein"; break;
				case TileType::Water: tileName = "Water"; break;
				default: tileName = "Unknown"; break;
				}
			}

			auto& cam = GraphicManager::GetInstance().camera;

			// RENDERING ************************************************************

			GraphicManager::GetInstance().BeginFrame();
			SDL_Renderer* r = GraphicManager::GetInstance().gRenderer;

			// Render all the items and the tiles of the map
			GridManager::GetInstance().RenderTiles();
			ObjectManager::GetInstance().RenderAll();
			ConveyorManager::GetInstance().RenderAll();

			GridManager::GetInstance().RenderDebugGrid(mouseX, mouseY);

			// Render the UI
			
			// Draw the boxes for the UI
			SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(r, 0, 0, 0, 160);
			SDL_Rect bg{ 0, 0, 500, 70 };
			SDL_RenderFillRect(r, &bg);
			bg = { SCREEN_WIDTH - 230, 0, 230, 70 };
			SDL_RenderFillRect(r, &bg);

			// Draw the text of the UI
			TextManager::GetInstance().DrawText(
				"Mouse cell (centered): (" + std::to_string(gridIdx.x) + ", " + std::to_string(gridIdx.y) + ")",
				white, uiFont, 10, 10, r);

			TextManager::GetInstance().DrawText(
				"Tile: " + tileName,
				white, uiFont, 10, 30, r);

			TextManager::GetInstance().DrawText(
				"Camera: (" + std::to_string((int)cam.GetX()) + ", " + std::to_string((int)cam.GetY()) + ")",
				white, uiFont, 10, 50, r);

			TextManager::GetInstance().DrawText(
				"FPS: " + std::to_string(fpsDisplay),
				white, uiFont, SCREEN_WIDTH - 130, 10, r);

			TextManager::GetInstance().DrawText(
				"Frame: " + std::to_string(msDisplay) + " ms",
				white, uiFont, SCREEN_WIDTH - 200, 30, r);


			GraphicManager::GetInstance().EndFrame();
		}

	}

	TextManager::GetInstance().CloseFont(uiFont);

	ObjectManager::GetInstance().Clear();
	AssetManager::GetInstance().Clear();
	GraphicManager::GetInstance().close();

	return 0;

}