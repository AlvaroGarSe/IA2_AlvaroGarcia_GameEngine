#pragma once
#include "SDL.h"
#include "Singleton.h"

class Camera2D
{
public:
	Camera2D();

	void SetPosition(float x, float y);
	void Move(float deltaX, float deltaY);

	SDL_Point WorldToScreen(float worldX, float worldY) const;
	SDL_Point ScreenToWorld(int screenX, int screenY) const;

	void SetWorldBounds(int worldW, int worldH, int screenW, int screenH);
	void SetViewPortSize(int screenW, int screenH);
	void Clamp();

	// Zoom
	void ZoomAtScreenPoint(float zoomFactor, int screenX, int screenY);
	float GetZoom() const { return mZoom; }
	void SetZoom(float zoom);
	void AddZoom(float deltaZoom);
	
	// Used to render the grid of the cells
	SDL_FRect GetWorldViewRect() const;

	float GetX() const { return camX; }
	float GetY() const { return camY; }

private:
	float camX = 0;
	float camY = 0;

	float mWorldW = 0;
	float mWorldH = 0;
	float mScreenW = 0;
	float mScreenH = 0;

	float mZoom = 1.0f;
	float mMinZoom = 0.5f;
	float mMaxZoom = 4.0f;

};
