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
#include "PlayerControlls.h"

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
	PlayerControlls::CreateSingleton();
	
	GraphicManager::GetInstance().setScreenSize(1366, 768);

	if (!GraphicManager::GetInstance().init())
	{
		printf("Failed to initialize!\n");
		return 0;
	}

	int SCREEN_WIDTH = GraphicManager::GetInstance().getScreenWidth();
	int SCREEN_HEIGHT = GraphicManager::GetInstance().getScreenHeight();


	TextManager::GetInstance().Init();
	RecipeManager::GetInstance().InitDefaults();

	TTF_Font* uiFont = TextManager::GetInstance().LoadFont("Media/Fonts/Ethnocentric-Regular.otf", 20);
	SDL_Color white{ 255,255,255,255 };

	ConveyorManager::GetInstance().Init(ConveyorMode::SoA);

	string conveyorMode;
	if (ConveyorManager::GetInstance().IsSoAMode())
	{
		conveyorMode = "SoA";
	}
	else
	{
		conveyorMode = "AoS";
	}

	if (!MapLoader::LoadMap("Media/Maps/map01.json"))
	{
		GridManager::GetInstance().CreateGrid(50, 50, 64);
		GridManager::GetInstance().SetTile(26, 26, TileType::IronVein);
	}

	int worldW = GridManager::GetInstance().GetWidth() * GridManager::GetInstance().GetCellSize();
	int worldH = GridManager::GetInstance().GetHeight() * GridManager::GetInstance().GetCellSize();
	int screenW = GraphicManager::GetInstance().getScreenWidth();
	int screenH = GraphicManager::GetInstance().getScreenHeight();

	GraphicManager::GetInstance().camera.SetWorldBounds(worldW, worldH, screenW, screenH);

	// Center the camera at the start of the game
	GraphicManager::GetInstance().camera.SetPosition((worldW - screenW) * 0.5f, (worldH - screenH) * 0.5f);

	// Basic buildings if not loaded buildings
	if (!MapLoader::LoadBuildings("Media/Maps/buildings01.json"))
	{
		GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().Spawn<Miner>(GameObject::Orientation::NORTH), 26, 26);
		GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().Spawn<Miner>(GameObject::Orientation::EAST), 26, 24);
		GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().Spawn<Crafter>(GameObject::Orientation::EAST, "iron_ingot"), 30, 25);
		GridManager::GetInstance().PlaceObject(ObjectManager::GetInstance().Spawn<Crafter>(GameObject::Orientation::EAST, "copper_ingot"), 30, 24);

		ConveyorManager::GetInstance().Create(20, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(21, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(22, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(23, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(24, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(25, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(26, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(27, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(28, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(29, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(31, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(32, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(33, 25, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(27, 24, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(28, 24, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(29, 24, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(31, 24, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(32, 24, GameObject::Orientation::EAST);
		ConveyorManager::GetInstance().Create(33, 24, GameObject::Orientation::EAST);

		ConveyorManager::GetInstance().Create(34, 24, GameObject::Orientation::SOUTH);
		ConveyorManager::GetInstance().Create(34, 25, GameObject::Orientation::SOUTH);
		ConveyorManager::GetInstance().Create(34, 26, GameObject::Orientation::SOUTH);
		ConveyorManager::GetInstance().Create(34, 27, GameObject::Orientation::SOUTH);
		ConveyorManager::GetInstance().Create(34, 28, GameObject::Orientation::SOUTH);
	}

	float camSpeed = GraphicManager::GetInstance().getScreenWidth() / 3 * GridManager::GetInstance().GetWidth() / 50; // pixels per second

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
		InputManager::GetInstance().BeginFrame();

		SDL_Event& e = *InputManager::GetInstance().GetSDLEvent();

		double dt = TimeManager::GetInstance().TickFrameSeconds();

		if (dt <= 0) dt = 0.001; // 1ms

		msDisplay = (int)(dt * 1000);

		// Calculate the FPS
		fpsTimer += dt;   // milliseconds
		fpsFrames++;

		if (fpsTimer >= 0.5) // 500 ms
		{
			fpsDisplay = (int)(fpsFrames / fpsTimer); // frames/sec
			fpsTimer = 0.0;
			fpsFrames = 0;
		}


		move = camSpeed * (float)dt;

		SDL_GetMouseState(&mouseX, &mouseY);

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

			InputManager::GetInstance().ProcessEvent(e);
		}

		// *************************************************************** INPUTS ***************************************************************

		// User can stop the game by pressing Escape
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		// Toggle the window to Fullscreen in borderless mode
		if (InputManager::GetInstance().GetKeyDown(SDL_SCANCODE_F11))
		{
			GraphicManager::GetInstance().ToggleFullscreen(true);
			SCREEN_WIDTH = GraphicManager::GetInstance().getScreenWidth();
			SCREEN_HEIGHT = GraphicManager::GetInstance().getScreenHeight();
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

		// Used to get the info of the selected cell for the inputs and the UI
		SDL_Point world = GraphicManager::GetInstance().camera.ScreenToWorld(mouseX, mouseY);
		SDL_Point gridIdx = GridManager::GetInstance().WorldToGrid(world.x, world.y);

		// Rotate building: <--Q || E-->
		if (InputManager::GetInstance().GetKeyDown(SDL_SCANCODE_E))
			PlayerControlls::GetInstance().RotateBuilding(gridIdx.x, gridIdx.y, 1);
		if (InputManager::GetInstance().GetKeyDown(SDL_SCANCODE_Q))
			PlayerControlls::GetInstance().RotateBuilding(gridIdx.x, gridIdx.y, -1);

		// Create buildings: 1 = Miner || 2 = Conveyor || 3 = Crafter
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_1))
			PlayerControlls::GetInstance().CreateMiner(gridIdx.x, gridIdx.y);
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_2))
			PlayerControlls::GetInstance().CreateConveyor(gridIdx.x, gridIdx.y);
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_3))
			PlayerControlls::GetInstance().CreateCrafter(gridIdx.x, gridIdx.y);

		// Remove buildings
		if (InputManager::GetInstance().GetKeyDown(SDL_SCANCODE_BACKSPACE) || InputManager::GetInstance().GetMouseButton(SDL_BUTTON_RIGHT))
			PlayerControlls::GetInstance().RemoveBuilding(gridIdx.x, gridIdx.y);
			
		// Change recipe of crafter
		if (InputManager::GetInstance().GetKeyDown(SDL_SCANCODE_R))
			PlayerControlls::GetInstance().ChangeCrafterRecipe(gridIdx.x, gridIdx.y);

		// Save the buildings placed on the map
		if (InputManager::GetInstance().GetKeyDown(SDL_SCANCODE_M))
			MapLoader::SaveBuildingsAsync("Media/Maps/buildings01.json");


		// *************************************************************** UPDATE ***************************************************************

		// Update all the items
		ObjectManager::GetInstance().UpdateAll();
		ConveyorManager::GetInstance().UpdateAll(TimeManager::GetInstance().getTicks());

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

		Camera2D& cam = GraphicManager::GetInstance().camera;

		// *************************************************************** RENDERING ************************************************************

		GraphicManager::GetInstance().BeginFrame();
		SDL_Renderer* r = GraphicManager::GetInstance().gRenderer;

		// Render all the items and the tiles of the map
		GridManager::GetInstance().RenderTiles();
		ObjectManager::GetInstance().RenderAll();
		ConveyorManager::GetInstance().RenderAll();

		GridManager::GetInstance().RenderDebugGrid(mouseX, mouseY);

			
		// ********************** Render the UI ******************************************+
		// Draw the boxes for the UI
		SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 160);
		SDL_Rect bg{ 0, 0, 500, 70 };
		SDL_RenderFillRect(r, &bg);
		bg = { SCREEN_WIDTH - 230, 0, 230, 70 };
		SDL_RenderFillRect(r, &bg);

		// Draw the text of the UI
		TextManager::GetInstance().DrawText("Mouse cell: (" + std::to_string(gridIdx.x) + ", " + std::to_string(gridIdx.y) + ")", white, uiFont, 10, 10, r);

		TextManager::GetInstance().DrawText("Tile: " + tileName, white, uiFont, 10, 30, r);

		TextManager::GetInstance().DrawText("Camera: (" + std::to_string((int)cam.GetX()) + ", " + std::to_string((int)cam.GetY()) + ")", white, uiFont, 10, 50, r);

		// FPS
		TextManager::GetInstance().DrawText("FPS: " + std::to_string(fpsDisplay), white, uiFont, SCREEN_WIDTH - 130, 10, r);

		// MS
		TextManager::GetInstance().DrawText("Frame: " + std::to_string(msDisplay) + " ms", white, uiFont, SCREEN_WIDTH - 200, 30, r);

		// Conveyor mode
		TextManager::GetInstance().DrawText("Mode: " + conveyorMode, white, uiFont, SCREEN_WIDTH - 200, 50, r);


		GraphicManager::GetInstance().EndFrame();
	}


	TextManager::GetInstance().CloseFont(uiFont);

	ObjectManager::GetInstance().Clear();
	AssetManager::GetInstance().Clear();

	GraphicManager::GetInstance().close();

	return 0;

}