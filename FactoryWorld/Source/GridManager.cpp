#include "GridManager.h"
#include "GraphicManager.h"

void GridManager::CreateGrid(int gridWidth, int gridHeight, int cellSize)
{
	mGridWidth = gridWidth;
	mGridHeight = gridHeight;

	mCellSize = cellSize;
	mCells.resize(gridWidth * gridHeight);
}

GridCell* GridManager::GetCell(int x, int y)
{
	if(!IsInside(x, y)) return nullptr;

	return &mCells[y * mGridWidth + x];
}

GridCell* GridManager::GetAdjacentCell(int x, int y, GameObject::Orientation orientation)
{
	switch (orientation)
	{
	case GameObject::Orientation::NONE:
		return nullptr;
		break;
	case GameObject::Orientation::NORTH:
		return GetCell(x, y - 1);
		break;
	case GameObject::Orientation::SOUTH:
		return GetCell(x, y + 1);
		break;
	case GameObject::Orientation::EAST:
		return GetCell(x + 1, y);
		break;
	case GameObject::Orientation::WEST:
		return GetCell(x - 1, y);
		break;
	default:
		return nullptr;
		break;
	}
}

bool GridManager::IsInside(int x, int y)
{
	return x >= 0 && y >=0 && x < mGridWidth && y < mGridHeight;
}

bool GridManager::CheckAdjacentCellEmpty(int x, int y, GameObject::Orientation orientation)
{
	switch (orientation)
	{
	case GameObject::Orientation::NONE:
		return true;
		break;
	case GameObject::Orientation::NORTH:
		return GetCell(x, y - 1)->gameObject == nullptr;
		break;
	case GameObject::Orientation::SOUTH:
		return GetCell(x, y + 1)->gameObject == nullptr;
		break;
	case GameObject::Orientation::EAST:
		return GetCell(x + 1, y)->gameObject == nullptr;
		break;
	case GameObject::Orientation::WEST:
		return GetCell(x - 1, y)->gameObject == nullptr;
		break;
	default:
		return true;
		break;
	}
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
	return GetCell(gridX, gridY)->gameObject == nullptr;
}

bool GridManager::PlaceObject(GameObject* obj, int gridX, int gridY)
{
	if (!CanPlace(gridX, gridY)) return false;

	GridCell* cell = GetCell(gridX, gridY);
	cell->gameObject = obj;

	SDL_Point worldPos = GridToWorld(gridX, gridY);
	obj->SetPosition(worldPos.x, worldPos.y);

	return true;
}

void GridManager::RemoveObject(int gridX, int gridY)
{
	if (!IsInside(gridX, gridY)) return;
	GetCell(gridX, gridY)->gameObject = nullptr;	
}

void GridManager::RenderDebugGrid()
{
	SDL_Renderer* renderer = GraphicManager::GetInstance().gRenderer;

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

	for (int y = 0; y < mGridHeight; y++)
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


