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
	GridCell* GetAdjacentCell(int cX, int cY, GameObject::Orientation orientation);
	bool IsInside(int x, int y) const;

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
	void RenderDebugGrid(int mouseX, int mouseY);
	void RenderSelectedCell(int mouseX, int mouseY, SDL_Renderer* gRenderer);
	void RenderTiles();

	bool CellIsEmpty(int gridX, int gridY);


	int GetCellSize() const { return mCellSize; }

	SDL_Point WorldToGridCameraRelative(int worldX, int worldY) const;
	SDL_Point GridCameraRelativeToWorld(int gx, int gy) const;

	// Transforms the coordinate centered to the relative not centered (0,0) -> (25,25)
	SDL_Point CenteredToIndex(int cx, int cy) const;

	// Transforms the coordinate not centered to centered (25,25) -> (0,0)
	SDL_Point IndexToCentered(int ix, int iy) const;

	bool CanPlaceCentered(int cx, int cy);
	bool PlaceObjectCentered(GameObject* obj, int cx, int cy);
	void RemoveObjectCentered(int cx, int cy);

	GridCell* GetCellCentered(int cx, int cy);
	GridCell* GetCellWorldPos(int worldX, int worldY);
	bool IsInsideCentered(int cx, int cy) const;

	void SetTile(int x, int y, TileType t);

	// Get the tile type of a cell position
	TileType GetTileWithCellPos(int cx, int cy) const;

	// Get the tile type of a world position
	TileType GetTileWithTransform(int x, int y) const;

	int GetWidth() const { return mGridWidth; }
	int GetHeight() const { return mGridHeight; }
};