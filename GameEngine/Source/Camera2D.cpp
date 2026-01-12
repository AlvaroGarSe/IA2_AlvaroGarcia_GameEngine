#include "Camera2D.h"

Camera2D::Camera2D()
{
	camX = 0;
	camY = 0;
}
void Camera2D::SetPosition(float x, float y)
{
	camX = x;
	camY = y;
	Clamp();
}

void Camera2D::Move(float deltaX, float deltaY)
{
	camX += deltaX;
	camY += deltaY;
	Clamp();
}

SDL_Point Camera2D::WorldToScreen(float worldX, float worldY) const
{
	float screenX = (worldX - camX) * mZoom;
	float screenY = (worldY - camY) * mZoom;
	return { (int)(screenX), (int)(screenY) };
}

SDL_Point Camera2D::ScreenToWorld(int screenX, int screenY) const
{
	float worldX = screenX / mZoom + camX;
	float worldY = screenY / mZoom + camY;
	return { (int) worldX, (int) worldY };
}

void Camera2D::SetWorldBounds(int worldW, int worldH, int screenW, int screenH)
{
	mWorldW = worldW;
	mWorldH = worldH;
	mScreenW = screenW;
	mScreenH = screenH;
	Clamp();
}

void Camera2D::SetViewPortSize(int screenW, int screenH)
{
	mScreenW = screenW;
	mScreenH = screenH;
	Clamp();
}

static float ClampF(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
void Camera2D::Clamp()
{
	// No clamping if dimensions are invalid
	if (mWorldW <= 0 || mWorldH <= 0 || mScreenW <= 0 || mScreenH <= 0) return;

	float viewW = mScreenW / mZoom;
	float viewH = mScreenH / mZoom;

	float maxX = (float) mWorldW - viewW;
	float maxY = (float) mWorldH - viewH;

	if (maxX < 0) maxX = 0;
	if (maxY < 0) maxY = 0;

	camX = ClampF(camX, 0.f, maxX);
	camY = ClampF(camY, 0.f, maxY);
}

void Camera2D::ZoomAtScreenPoint(float zoomFactor, int screenX, int screenY)
{
	// World point under mouse before zoom
	SDL_Point before = ScreenToWorld(screenX, screenY);

	// Apply zoom
	SetZoom(mZoom * zoomFactor);

	// World point under mouse after zoom
	SDL_Point after = ScreenToWorld(screenX, screenY);

	// Shift camera so the same world point stays under mouse
	camX += (before.x - after.x);
	camY += (before.y - after.y);

	Clamp();
}

void Camera2D::SetZoom(float zoom)
{
	mZoom = ClampF(zoom, mMinZoom, mMaxZoom);
	Clamp();
}

void Camera2D::AddZoom(float deltaZoom)
{
	SetZoom(mZoom + deltaZoom);
}

