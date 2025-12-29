#pragma once

#include <vector>
#include "Singleton.h"
#include "GridCell.h"
#include "GameObject.h"
#include "SDL.h"


class GridManager :public Singleton<GridManager>
{
	/*****************************************************************************/
	friend class Singleton<GridManager>;
	/*****************************************************************************/

private:
	GridManager() {};

	// Number of cells in each direction
	int mGridWidth = 0;
	int mGridHeight = 0;

	int mCellSize = 0;

	std::vector<GridCell> mCells;

public:
	void CreateGrid(int gridWidth, int gridHeight, int cellSize);

	GridCell* GetCell(int x, int y);
	GridCell* GetAdjacentCell(int x, int y, GameObject::Orientation orientation);
	bool IsInside(int x, int y);

	// Checks if the adjacent cell in the given orientation is empty (true = empty)
	bool CheckAdjacentCellEmpty(int x, int y, GameObject::Orientation orientation);

	// Conversion helpers
	SDL_Point WorldToGrid(int worldX, int worldY) const;
	SDL_Point GridToWorld(int gridX, int gridY) const;

	// Placement
	bool CanPlace(int gridX, int gridY);
	bool PlaceObject(GameObject* obj, int gridX, int gridY);
	void RemoveObject(int gridX, int gridY);

	// Debug
	void RenderDebugGrid();

	int GetCellSize() const { return mCellSize; }


};