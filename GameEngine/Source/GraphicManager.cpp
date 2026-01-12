#include "GraphicManager.h"
#include "Camera2D.h"
#include <iostream>

bool GraphicManager::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void GraphicManager::close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void GraphicManager::BeginFrame()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
}

void GraphicManager::EndFrame()
{
	SDL_RenderPresent(gRenderer);
}

void GraphicManager::setScreenSize(int screenWidth, int screenHeight)
{
	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;

	if (gWindow && !IsFullscreen())
		SDL_SetWindowSize(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool GraphicManager::SetFullscreen(bool enabled, bool borderless)
{
	if (!gWindow) return false;

	Uint32 flag = 0;
	if (enabled)
		flag = borderless ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN;

	if (SDL_SetWindowFullscreen(gWindow, flag) != 0)
	{
		std::cout << "SDL_SetWindowFullscreen error: " << SDL_GetError() << "\n";
		return false;
	}

	// Update cached size
	RefreshWindowSizeCache();

	camera.SetViewPortSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return true;
}

void GraphicManager::ToggleFullscreen(bool borderless)
{
	SetFullscreen(!IsFullscreen(), borderless);
}

bool GraphicManager::IsFullscreen() const
{
	if (!gWindow) return false;
	Uint32 f = SDL_GetWindowFlags(gWindow);
	return (f & SDL_WINDOW_FULLSCREEN) || (f & SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void GraphicManager::RefreshWindowSizeCache()
{
	if (!gWindow) return;
	int w = 0, h = 0;
	SDL_GetWindowSize(gWindow, &w, &h);
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;
}

void GraphicManager::DrawTexture(LTexture* texture, const Transform& transform, SDL_Rect* clip, SDL_RendererFlip flip)
{
	float zoom = camera.GetZoom();

	SDL_Point screenPos = camera.WorldToScreen((int)transform.x, (int)transform.y);

	//Set rendering space and render to screen
	SDL_Rect renderQuad = {screenPos.x, screenPos.y, texture->getWidth() * transform.scaleX * zoom, texture->getHeight() * transform.scaleY * zoom};

	SDL_Point center;
	center.x = renderQuad.w / 2;
	center.y = renderQuad.h / 2;

	//Render to screen
	SDL_RenderCopyEx(gRenderer, texture->getSDLTexture(), clip, &renderQuad, transform.rotation, &center, flip);

}

void GraphicManager::DrawTextureSoA(LTexture* texture, int x, int y, double rotation, float scaleX, float scaleY, SDL_Rect* clip, SDL_RendererFlip flip)
{
	float zoom = camera.GetZoom();

	SDL_Point screenPos = camera.WorldToScreen((int)x, (int)y);

	//Set rendering space and render to screen
	SDL_Rect renderQuad = { screenPos.x, screenPos.y, texture->getWidth() * scaleX * zoom, texture->getHeight() * scaleY * zoom };

	SDL_Point center;
	center.x = renderQuad.w / 2;
	center.y = renderQuad.h / 2;

	//Render to screen
	SDL_RenderCopyEx(gRenderer, texture->getSDLTexture(), clip, &renderQuad, rotation, &center, flip);
}
