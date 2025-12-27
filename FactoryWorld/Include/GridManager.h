#pragma once

#include <vector>
#include "Singleton.h"
#include "GridCell.h"
#include "SDL.h"


class GridManager :public Singleton<GridManager>
{
	/*****************************************************************************/
	friend class Singleton<GridManager>;
	/*****************************************************************************/

private:
	GridManager() {};

	int mGridWidth = 0;
	int mGridHeight = 0;
	int mCellSize = 0;

	std::vector<GridCell> mCells;

public:
	void CreateGrid(int gridWidth, int gridHeight, int cellSize);

	GridCell& GetCell(int x, int y);
	bool IsInside(int x, int y);

	// Conversion helpers
	SDL_Point WorldToGrid(int worldX, int worldY) const;
	SDL_Point GridToWorld(int gridX, int gridY) const;

	// Placement
	bool CanPlace(int gridX, int gridY);
	bool PlaceObject(GameObject* obj, int gridX, int gridY);
	void RemoveObject(int gridX, int gridY);

	// Debug
	void RenderDebugGrid();


};