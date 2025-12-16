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
#include "Ship.h"
#include "Background.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "PhysicsManager.h"
#include "TextManager.h"

#include "iostream"
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
	
	// Create the background adn the ship
	Background background = Background(SCREEN_WIDTH, SCREEN_HEIGHT, "../../03-Media/bgSpace.jpg");
	Ship ship = Ship(GraphicManager::GetInstance().getScreenWidth(), GraphicManager::GetInstance().getScreenHeight());

	// Pool of the projectiles of the ship
	const int NUMBER_MAX_PROJECTILES = 20;
	Projectile* Projectiles[NUMBER_MAX_PROJECTILES];

	// Pool of the asteroids
	const int NUMBER_MAX_ASTEROIDS = 20;
	Asteroid* Asteroids[NUMBER_MAX_ASTEROIDS];

	bool createProjectile = false;
	bool createAsteroid = false;
	
	// Used for the shooting delay and the spawn rate of the asteroids
	const Uint32 shootDelayMs = 800; // In milliseconds
	Uint32 lastTimeShoot = shootDelayMs;
	const Uint32 asteroidDelayMs = 600; // In milliseconds
	Uint32 lastAsteroidSpawn = asteroidDelayMs;
	Uint32 Time = 0;

	// Score of the player
	int score = 0;
	
	// I tried to add all the objects in the objects manager but i couldnt do it properly

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
		// Load media of all the objects //

		// Add the Projectiles in the pool
		for (int i = 0; i < NUMBER_MAX_PROJECTILES; i++)
		{
			Projectiles[i] = new Projectile(ship.mWidth, ship.mPosX, ship.mPosY);
			Projectiles[i]->loadMedia();
		}
		// Add the Asteroids in the pool
		for (int i = 0; i < NUMBER_MAX_ASTEROIDS; i++)
		{
			Asteroids[i] = new Asteroid(SCREEN_WIDTH, SCREEN_HEIGHT);
			Asteroids[i]->loadMedia();
		}
		background.loadMedia();
		if( !ship.loadMedia() )
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

				// Get the time
				Time = TimeManager::GetInstance().getTicks();

				//Handle events on queue
				while( SDL_PollEvent(InputManager::GetInstance().GetSDLEvent()) != 0)
				{
					//User requests quit
					if(InputManager::GetInstance().GetSDLEvent()->type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the ship
					ship.handleEvent(InputManager::GetInstance());

					// User can stop the game by pressing Escape
					if (InputManager::GetInstance().GetKey(SDL_SCANCODE_ESCAPE))
					{
						quit = true;
					}

					// User can shoot a projectile when the Space is pressed and when there is no delay
					if (InputManager::GetInstance().GetKey(SDL_SCANCODE_SPACE))
					{
						createProjectile = true;
						// Gets a viable Projectile from the pool and activates it
						for (int i = 0; i < NUMBER_MAX_PROJECTILES && createProjectile && Time -  lastTimeShoot > shootDelayMs; i++)
						{
							if (!Projectiles[i]->isActive)
							{
								Projectiles[i]->GenerateProjectile(ship.mWidth, ship.mPosX, ship.mPosY);
								createProjectile = false;
								lastTimeShoot = Time;
							}
						}
					}
				}

				// Spawns the Asteroids taking them from the pool
				if (Time - lastAsteroidSpawn > asteroidDelayMs)
				{
					createAsteroid = true;
					for (int i = 0; i < NUMBER_MAX_ASTEROIDS && createAsteroid; i++)
					{
						if (!Asteroids[i]->isActive)
						{
							Asteroids[i]->GenerateAsteroid();
							createAsteroid = false;
							lastAsteroidSpawn = Time;
						}
					}
				}

				// Move all the objects //

				ship.move();
				for (int i = 0; i < NUMBER_MAX_ASTEROIDS; i++)
				{
					if (Asteroids[i]->isActive)
					{
						// Move the asteroids
						Asteroids[i]->move();

						// Check the collission between the ship and the asteroids using the Physics Manager
						if (PhysicsManager::GetInstance().checkCollision(*Asteroids[i], ship))
						{
							quit = true;
						}
						// Check the collission between the projectiles and the asteroids using the Physics Manager
						for (int j = 0; j < NUMBER_MAX_PROJECTILES; j++)
						{
							if (Projectiles[j]->isActive)
							{
								if (PhysicsManager::GetInstance().checkCollision(*Asteroids[i], *Projectiles[j]))
								{
									//cout << "collision";
									Projectiles[j]->isActive = false;
									Asteroids[i]->isActive = false;
									score++;
								}
							}
						}
					}
				}
				// Move all the Projectiles
				for (int i = 0; i < NUMBER_MAX_PROJECTILES; i++)
				{
					Projectiles[i]->move();
				}

				// Rendering of all the objects //

				background.render();
				for (int i = 0; i < NUMBER_MAX_PROJECTILES; i++)
				{
					Projectiles[i]->render();
				}
				for (int i = 0; i < NUMBER_MAX_ASTEROIDS; i++)
				{
					Asteroids[i]->render();
				}
				ship.render();

				SDL_Color textColor = { 255, 229, 130, 255 };
				TextManager::GetInstance().renderFont(textColor, font, 20, 20, score, GraphicManager::GetInstance().gRenderer);

				SDL_RenderPresent(GraphicManager::GetInstance().gRenderer);
			}
		}
	}

	//Free all the objects and close SDL
	GraphicManager::GetInstance().close();
	ship.getShipTexture()->free();
	for (int i = 0; i < NUMBER_MAX_PROJECTILES; i++)
	{
		Projectiles[i]->getProjectileTexture()->free();
	}
	for (int i = 0; i < NUMBER_MAX_ASTEROIDS; i++)
	{
		Asteroids[i]->getAsteroidTexture()->free();
	}
	TextManager::GetInstance().closeFont(font);
	return 0;
}