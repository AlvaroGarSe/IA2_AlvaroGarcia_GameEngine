#include "GridManager.h"
#include "GraphicManager.h"

void GridManager::CreateGrid(int gridWidth, int gridHeight, int cellSize)
{
	mGridWidth = gridWidth;
	mGridHeight = gridHeight;

	mCellSize = cellSize;
	mCells.resize(gridWidth * gridHeight);
}

GridCell& GridManager::GetCell(int x, int y)
{
	return mCells[y * mGridWidth + x];
}

bool GridManager::IsInside(int x, int y)
{
	return x >= 0 && y >=0 && x < mGridWidth && y < mGridHeight;
}

SDL_Point GridManager::WorldToGrid(int worldX, int worldY) const
{
	return { worldX / mCellSize , worldY / mCellSize };
}

SDL_Point GridManager::GridToWorld(int gridX, int gridY) const
{
	return { gridX * mCellSize , gridY * mCellSize };
}

bool GridManager::CanPlace(int gridX, int gridY)
{
	if (!IsInside(gridX, gridY)) return false;
	return GetCell(gridX, gridY).gameObject == nullptr;
}

bool GridManager::PlaceObject(GameObject* obj, int gridX, int gridY)
{
	if (!CanPlace(gridX, gridY)) return false;

	GridCell& cell = GetCell(gridX, gridY);
	cell.gameObject = obj;

	SDL_Point worldPos = GridToWorld(gridX, gridY);
	obj->SetPosition(worldPos.x, worldPos.y);

	return true;
}

void GridManager::RemoveObject(int gridX, int gridY)
{
	if (!IsInside(gridX, gridY)) return;
	GetCell(gridX, gridY).gameObject = nullptr;	
}

void GridManager::RenderDebugGrid()
{
	SDL_Renderer* renderer = GraphicManager::GetInstance().gRenderer;

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

	for (size_t y = 0; y < mGridHeight; y++)
	{
		for (int x = 0; x < mGridWidth; x++)
		{
			SDL_Rect rect;
			rect.x = x * mCellSize;
			rect.y = y * mCellSize;
			rect.w = mCellSize;
			rect.h = mCellSize;

			SDL_RenderDrawRect(renderer, &rect);
		}

	}
}


