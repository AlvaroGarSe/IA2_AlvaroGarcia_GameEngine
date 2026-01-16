#include "GridManager.h"
#include "GraphicManager.h"
#include "Camera2D.h"
#include "AssetManager.h"
#include "ConveyorManager.h"
#include "ObjectManager.h"

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

GridCell* GridManager::GetAdjacentCell(int cX, int cY, GameObject::Orientation orientation)
{
	switch (orientation)
	{
	case GameObject::Orientation::NONE:
		return nullptr;
		break;
	case GameObject::Orientation::NORTH:
		return GetCell(cX, cY - 1);
		break;
	case GameObject::Orientation::SOUTH:
		return GetCell(cX, cY + 1);
		break;
	case GameObject::Orientation::EAST:
		return GetCell(cX + 1, cY);
		break;
	case GameObject::Orientation::WEST:
		return GetCell(cX - 1, cY);
		break;
	default:
		return nullptr;
		break;
	}
}

bool GridManager::IsInside(int x, int y) const
{
	return x >= 0 && y >=0 && x < mGridWidth && y < mGridHeight;
}

bool GridManager::CheckAdjacentCellEmpty(int x, int y, GameObject::Orientation orientation)
{
	GridCell* adj = GetAdjacentCell(x, y, orientation);
	if (!adj) return false;
	return adj->gameObject == nullptr;
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
	GridCell* cell = GetCell(gridX, gridY);
	//  If the tile is a water tile, it can not be constructed there
	if (cell->tileType == TileType::Water) return false;
	return cell->gameObject == nullptr && cell->conveyorId == INVALID_CONVEYOR;
}

// Uses cells position
bool GridManager::PlaceObject(GameObject* obj, int gridX, int gridY)
{
	if (!CanPlace(gridX, gridY)) return false;

	GridCell* cell = GetCell(gridX, gridY);
	cell->gameObject = obj;

	SDL_Point worldPos = GridToWorld(gridX, gridY);
	obj->SetPosition(worldPos.x, worldPos.y);
	return true;
}

bool GridManager::RemoveBuilding(int gridX, int gridY)
{
	GridCell* cell = GetCell(gridX, gridY);
	if (!cell) return false;

	if (cell->conveyorId != INVALID_CONVEYOR)
	{
		ConveyorManager::GetInstance().RemoveConveyor(cell->conveyorId);
		cell->conveyorId = INVALID_CONVEYOR;
		return true;
	}
	if (cell->gameObject != nullptr)
	{
		GameObject* obj = cell->gameObject;
		cell->gameObject = nullptr;
		ObjectManager::GetInstance().Destroy(obj);
		return true;
	}
		
	return false;
}

void GridManager::RenderDebugGrid(int mouseX, int mouseY)
{
	SDL_Renderer* r = GraphicManager::GetInstance().gRenderer;
	Camera2D& cam = GraphicManager::GetInstance().camera;

	SDL_SetRenderDrawColor(r, 50, 50, 50, 255);

	const SDL_FRect worldView = cam.GetWorldViewRect();

	int minX = (int)floorf(worldView.x / mCellSize);
	int minY = (int)floorf(worldView.y / mCellSize);
	int maxX = (int)ceilf((worldView.x + worldView.w) / mCellSize);
	int maxY = (int)ceilf((worldView.y + worldView.h) / mCellSize);

	// Clamp
	minX = std::max(0, minX);
	minY = std::max(0, minY);
	maxX = std::min(mGridWidth, maxX);
	maxY = std::min(mGridHeight, maxY);

	// Draw vertical lines
	for (int x = minX; x <= maxX; ++x)
	{
		float wx = (float)(x * mCellSize);
		SDL_Point a = cam.WorldToScreen(wx, (float)(minY * mCellSize));
		SDL_Point b = cam.WorldToScreen(wx, (float)(maxY * mCellSize));
		SDL_RenderDrawLine(r, a.x, a.y, b.x, b.y);
	}

	// Draw horizontal lines
	for (int y = minY; y <= maxY; ++y)
	{
		float wy = (float)(y * mCellSize);
		SDL_Point a = cam.WorldToScreen((float)(minX * mCellSize), wy);
		SDL_Point b = cam.WorldToScreen((float)(maxX * mCellSize), wy);
		SDL_RenderDrawLine(r, a.x, a.y, b.x, b.y);
	}

	RenderSelectedCell(mouseX, mouseY, r);
}

void GridManager::RenderSelectedCell(int mouseX, int mouseY, SDL_Renderer* gRenderer)
{
	SDL_Point world = GraphicManager::GetInstance().camera.ScreenToWorld(mouseX, mouseY);
	SDL_Point absGrid = WorldToGrid(world.x, world.y);

	// Checks if mouse is inside grid
	if (!IsInside(absGrid.x, absGrid.y)) return;

	SDL_Point gRel = GridManager::GetInstance().WorldToGridCameraRelative(world.x, world.y);

	SDL_Point cellWorld = GridManager::GetInstance().GridCameraRelativeToWorld(gRel.x, gRel.y);

	SDL_Point screen = GraphicManager::GetInstance().camera.WorldToScreen((float)(cellWorld.x), (float)(cellWorld.y));

	float z = GraphicManager::GetInstance().camera.GetZoom();

	SDL_Rect r{ screen.x, screen.y, (int)(mCellSize * z), (int)(mCellSize * z) };
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(gRenderer, &r);
}

static const char* TilePath(TileType t)
{
	switch (t)
	{
	case TileType::Ground:
		return "Media/Tiles/Ground.png";
	case TileType::IronVein:
		return "Media/Tiles/IronVein.png";
	case TileType::CopperVein:
		return "Media/Tiles/CooperVein.png";
	case TileType::Water:
		return "Media/Tiles/Water.png";
	default:
		return "Media/Tiles/Ground.png";
	}
}

void GridManager::RenderTiles()
{
	auto& gfx = GraphicManager::GetInstance();
	auto& cam = gfx.camera;

	// Get visible world rectangle (top-left and bottom-right in world coords)
	SDL_Point worldTL = cam.ScreenToWorld(0, 0);
	SDL_Point worldBR = cam.ScreenToWorld(gfx.getScreenWidth(), gfx.getScreenHeight());

	// Convert to grid coords
	SDL_Point gridTL = WorldToGrid(worldTL.x, worldTL.y);
	SDL_Point gridBR = WorldToGrid(worldBR.x, worldBR.y);

	// Expand a bit so edges don’t pop
	gridTL.x -= 1; gridTL.y -= 1;
	gridBR.x += 1; gridBR.y += 1;

	// Clamp to grid
	if (gridTL.x < 0) gridTL.x = 0;
	if (gridTL.y < 0) gridTL.y = 0;
	if (gridBR.x >= mGridWidth)  gridBR.x = mGridWidth - 1;
	if (gridBR.y >= mGridHeight) gridBR.y = mGridHeight - 1;

	LTexture* ground = AssetManager::GetInstance().GetTexture(TilePath(TileType::Ground));
	LTexture* iron = AssetManager::GetInstance().GetTexture(TilePath(TileType::IronVein));
	LTexture* copper = AssetManager::GetInstance().GetTexture(TilePath(TileType::CopperVein));
	LTexture* water = AssetManager::GetInstance().GetTexture(TilePath(TileType::Water));

	// Draw visible tiles
	for (int y = gridTL.y; y <= gridBR.y; ++y)
	{
		for (int x = gridTL.x; x <= gridBR.x; ++x)
		{
			GridCell* cell = GetCell(x, y);
			if (!cell) continue;
			
			LTexture* tex = nullptr;

			switch (cell->tileType)
			{
			case TileType::Ground:
				tex = ground;
				break;
			case TileType::IronVein:
				tex = iron;
				break;
			case TileType::CopperVein:
				tex = copper;
				break;
			case TileType::Water:
				tex = water;
				break;
			default:
				tex = ground;
				break;
			}

			if (!tex) continue;

			SDL_Point worldPos = GridToWorld(x, y);

			// Draw tile scaled to cell size (world)
			Transform t;
			t.x = (float)worldPos.x;
			t.y = (float)worldPos.y;
			t.rotation = 0.0f;

			// scale so that texture fills one cell in world space
			t.scaleX = (float)mCellSize / (float)tex->getWidth();
			t.scaleY = (float)mCellSize / (float)tex->getHeight();

			gfx.DrawTile(tex, t);
		}
	}
}

bool GridManager::CellIsEmpty(int gridX, int gridY)
{
	if (!IsInside(gridX, gridY)) return false;
	GridCell* cell = GetCell(gridX, gridY);
	return cell->gameObject == nullptr && cell->conveyorId == INVALID_CONVEYOR;
}

SDL_Point GridManager::WorldToGridCameraRelative(int worldX, int worldY) const
{
	// 1) celda bajo el mouse
	SDL_Point grid = WorldToGrid(worldX, worldY);

	// 2) centro de la pantalla en world
	const Camera2D& cam = GraphicManager::GetInstance().camera;
	int screenW = GraphicManager::GetInstance().getScreenWidth();
	int screenH = GraphicManager::GetInstance().getScreenHeight();

	int centerWorldX = (int)(cam.GetX() + screenW * 0.5f);
	int centerWorldY = (int)(cam.GetY() + screenH * 0.5f);

	// 3) celda central
	SDL_Point centerGrid = WorldToGrid(centerWorldX, centerWorldY);

	// 4) coordenadas relativas
	return { grid.x - centerGrid.x, grid.y - centerGrid.y };
}

SDL_Point GridManager::GridCameraRelativeToWorld(int gx, int gy) const
{
	const Camera2D& cam = GraphicManager::GetInstance().camera;
	int screenW = GraphicManager::GetInstance().getScreenWidth();
	int screenH = GraphicManager::GetInstance().getScreenHeight();

	int centerWorldX = (int)(cam.GetX() + screenW * 0.5f);
	int centerWorldY = (int)(cam.GetY() + screenH * 0.5f);

	SDL_Point centerGrid = WorldToGrid(centerWorldX, centerWorldY);

	// grid absoluto
	int absX = centerGrid.x + gx;
	int absY = centerGrid.y + gy;

	return GridToWorld(absX, absY);
}

SDL_Point GridManager::CenteredToIndex(int cx, int cy) const
{
	return { cx + mGridWidth / 2, cy + mGridHeight / 2 };
}

SDL_Point GridManager::IndexToCentered(int ix, int iy) const
{
	return { ix - mGridWidth / 2, iy - mGridHeight / 2 };
}

GridCell* GridManager::GetCellCentered(int cx, int cy)
{
	SDL_Point idx = CenteredToIndex(cx, cy);
	return GetCell(idx.x, idx.y);
}

GridCell* GridManager::GetCellWorldPos(int worldX, int worldY)
{
	if (!IsInside(worldX, worldY)) return nullptr;
	SDL_Point cellCoords = WorldToGrid(worldX, worldY);
	return GetCell(cellCoords.x, cellCoords.y);
}

bool GridManager::IsInsideCentered(int cx, int cy) const
{
	SDL_Point idx = CenteredToIndex(cx, cy);
	return IsInside(idx.x, idx.y);
}

void GridManager::SetTile(int x, int y, TileType t)
{
	if (!IsInside(x, y)) return;
	GetCell(x, y)->tileType = t;
}

TileType GridManager::GetTileWithCellPos(int cx, int cy) const
{
	if (!IsInside(cx, cy)) return TileType::Ground;
	return mCells[cy * mGridWidth + cx].tileType;
}

TileType GridManager::GetTileWithTransform(int x, int y) const
{
	SDL_Point cellCoords = WorldToGrid(x, y);

	if (!IsInside(cellCoords.x, cellCoords.y)) return TileType::Ground;
	return mCells[cellCoords.y * mGridWidth + cellCoords.x].tileType;
}