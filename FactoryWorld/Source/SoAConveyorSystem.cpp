#include "SoAConveyorSystem.h"
#include "TimeManager.h"
#include "GridManager.h"
#include "GraphicManager.h"
#include "AssetManager.h"
#include "Crafter.h"

SoAConveyorSystem::SoAConveyorSystem()
{
	convTexture = AssetManager::GetInstance().GetTexture("Media/Conveyor.png");
}

ConveyorId SoAConveyorSystem::Create(int cX, int cY, GameObject::Orientation o)
{
	// Calculate the world position of the conveyor from the given cell position(cX, cY)
	SDL_Point worldPos = GridManager::GetInstance().GridToWorld(cX, cY);

	mX.push_back(worldPos.x);
	mY.push_back(worldPos.y);
	mOri.push_back(o);
	
	mNextMoveTime.push_back(0);
	mActive.push_back(false);
	
	// Make the slots empty of the conveyor
	for (int i = 0; i < SLOT_COUNT; i++)
	    mSlotType.push_back(ItemType::NONE);

	// id = index+1
	ConveyorId id = (ConveyorId)mX.size();

	if (GridCell* cell = GridManager::GetInstance().GetCell(cX, cY))
	{
		cell->conveyorId = id;
	}
	
	return (ConveyorId) id;
}

void SoAConveyorSystem::StartAll()
{
	for (int i = 0; i < mActive.size(); i++)
	{
		mActive[i] = true;
	}
}

void SoAConveyorSystem::UpdateAll(uint32_t now)
{
	for (int i = 0; i < mX.size(); ++i)
	{
		if (!mActive[i]) continue;

		// Skips if no items in any slot
		bool hasItem = false;
		for (int s = 0; s < SLOT_COUNT; ++s)
		{
			if (mSlotType[SlotToIndex(i, s)] != ItemType::NONE) { hasItem = true; break; }
		}
		if (!hasItem) continue;

		if (now < mNextMoveTime[i]) continue;

		mNextMoveTime[i] = now + mMoveIntervalMs;

		PassItemNextCell(i);
		MoveItemThroughSlots(i);
	}
}

static const char* ItemSpritePath(ItemType t)
{
	switch (t)
	{
		case ItemType::IronOre:   return "Media/Items/IronOre.png";
		case ItemType::IronIngot: return "Media/Items/IronIngot.png";
		case ItemType::CopperOre:   return "Media/Items/CopperOre.png";
		case ItemType::CopperIngot: return "Media/Items/CopperIngot.png";
		case ItemType::IronRod: return "Media/Items/IronRod.png";
		case ItemType::IronWrench: return "Media/Items/IronWrench.png";
		case ItemType::IronGear: return "Media/Items/IronGear.png";
		case ItemType::CopperGear: return "Media/Items/CopperGear.png";
		default:                  return "Media/Items/UnknownItem.png";
	}
}

void SoAConveyorSystem::RenderAll()
{
	GraphicManager& gfx = GraphicManager::GetInstance();

	const int cellSize = GridManager::GetInstance().GetCellSize();

	double rotation = 0.0;
	float scaleX = 0.5f;
	float scaleY = 0.5f;

	// Renders first all the Conveyor sprites to not render some items behind the next conveyor
	for (int i = 0; i < mX.size(); ++i)
	{
		if (!mActive[i]) continue;

		// Direction of the sprite deppending on the orientation
		switch (mOri[i])
		{
		case GameObject::Orientation::NORTH: rotation = 0; break;
		case GameObject::Orientation::SOUTH: rotation = 180; break;
		case GameObject::Orientation::EAST:  rotation = 90;  break;
		case GameObject::Orientation::WEST:  rotation = 270; break;
		default: rotation = 0; break;
		}

		gfx.DrawTextureSoA(convTexture, mX[i], mY[i], rotation, scaleX, scaleY);

	}

	// World-space item size (in "world pixels")
	const int itemSizeWorld = 32;
	const int slotSpacingWorld = cellSize / SLOT_COUNT;

	const int centerOffsetWorld = (int)((cellSize - itemSizeWorld / 2) / 2);

	int dirX = 0, dirY = 0;
	
	// Parameters for the transform of the items
	int startX, startY;
	float sx, sy, offset;

	// Rendering of all the items on the conveyors
	for (int i = 0; i < mX.size(); ++i)
	{
		if (!mActive[i]) continue;

		// Direction of the sprite deppending on the orientation
		switch (mOri[i])
		{
		case GameObject::Orientation::NORTH: dirX = 0;  dirY = -1; break;
		case GameObject::Orientation::SOUTH: dirX = 0;  dirY = 1; break;
		case GameObject::Orientation::EAST:  dirX = 1;  dirY = 0; break;
		case GameObject::Orientation::WEST:  dirX = -1; dirY = 0; break;
		default: break;
		}

		startX = mX[i] + centerOffsetWorld;
		startY = mY[i] + centerOffsetWorld;

		for (int s = 0; s < SLOT_COUNT; ++s)
		{
			ItemType t = mSlotType[SlotToIndex(i, s)];
			if (t == ItemType::NONE) continue;

			LTexture* itemTex = AssetManager::GetInstance().GetTexture(ItemSpritePath(t));
			if (!itemTex) continue;

			sx = (float)itemSizeWorld / (float)itemTex->getWidth();
			sy = (float)itemSizeWorld / (float)itemTex->getHeight();

			offset = (s * slotSpacingWorld) - (cellSize / 2.0f - slotSpacingWorld / 2.0f);

			gfx.DrawTextureSoA(itemTex, (int)(startX + dirX * offset - itemSizeWorld * 0.25f), (int)(startY + dirY * offset - itemSizeWorld * 0.25f), 0.0f, sx, sy);
		}
	}
}

ConveyorId SoAConveyorSystem::CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o)
{
	if (!GridManager::GetInstance().CanPlace(cX, cY)) return INVALID_CONVEYOR;
	int id = IndexFromId(Create(cX, cY, o));
	mActive[id] = true;
	return mActive[id] = true;
}

bool SoAConveyorSystem::CanAcceptItem(ConveyorId id) const
{
	int i = IndexFromId(id);
	    if (i < 0 || i >= mX.size()) return false;
	
	    return SlotEmpty(i, 0);
}

bool SoAConveyorSystem::InsertItem(ConveyorId id, const Item& item)
{
	if(!CanAcceptItem(id)) return false;
	
	int i = IndexFromId(id);
	SetSlot(i, 0, item.type);
	uint32_t now = TimeManager::GetInstance().getTicks();
	if (mNextMoveTime[i] <= now) mNextMoveTime[i] = now + mMoveIntervalMs;

	return true;
}

void SoAConveyorSystem::RotateConveyor(ConveyorId id, int dir)
{
	GameObject::Orientation& ori = mOri[IndexFromId(id)];
	if (dir == 1)
	{
		switch (ori)
		{
		case GameObject::Orientation::NORTH: ori = GameObject::Orientation::EAST; break;
		case GameObject::Orientation::EAST: ori = GameObject::Orientation::SOUTH; break;
		case GameObject::Orientation::SOUTH: ori = GameObject::Orientation::WEST; break;
		case GameObject::Orientation::WEST: ori = GameObject::Orientation::NORTH; break;
		default:
			ori = GameObject::Orientation::NONE;
			break;
		}
	}
	else if (dir == -1)
	{
		switch (ori)
		{
		case GameObject::Orientation::NORTH: ori = GameObject::Orientation::WEST; break;
		case GameObject::Orientation::WEST: ori = GameObject::Orientation::SOUTH; break;
		case GameObject::Orientation::SOUTH: ori = GameObject::Orientation::EAST; break;
		case GameObject::Orientation::EAST: ori = GameObject::Orientation::NORTH; break;
		default:
			ori = GameObject::Orientation::NONE;
			break;
		}
	}
}

void SoAConveyorSystem::RemoveConveyor(ConveyorId id)
{
	int i = IndexFromId(id);
	if (i < 0 || i >= mX.size()) return;

	for (int s = 0; s < SLOT_COUNT; ++s)
		mSlotType[SlotToIndex(i, s)] = ItemType::NONE;

	mActive[i] = false;
	mNextMoveTime[i] = 0;
}

SDL_Point SoAConveyorSystem::GetWorldPos(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= mX.size()) return SDL_Point{ 0,0 };
	return SDL_Point{ mX[i], mY[i] };
}

GameObject::Orientation SoAConveyorSystem::GetOrientation(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= mOri.size()) return GameObject::Orientation::NONE;
	return mOri[i];
}

void SoAConveyorSystem::MoveItemThroughSlots(int i)
{
	// From back to front: [2]->[3], [1]->[2], [0]->[1]
	for (int s = SLOT_COUNT - 1; s > 0; --s)
	{
		int dst = SlotToIndex(i, s);
		int src = dst - 1;

		if (mSlotType[dst] == ItemType::NONE && mSlotType[src] != ItemType::NONE)
		{
			mSlotType[dst] = mSlotType[src];
			mSlotType[src] = ItemType::NONE;
		}
	}
}

void SoAConveyorSystem::PassItemNextCell(int i)
{
	int lastIdx = SlotToIndex(i, SLOT_COUNT - 1);

	// If last slot is empty, nothing to pass
	ItemType t = mSlotType[lastIdx];
	if (t == ItemType::NONE) return;

	// Current conveyor grid cell from world pos
	GridManager& grid = GridManager::GetInstance();

	SDL_Point gridPos = grid.WorldToGrid(mX[i], mY[i]);
	GridCell* nextCell = grid.GetAdjacentCell(gridPos.x, gridPos.y, mOri[i]);

	if (!nextCell) return;

	// Case A: next cell has a SoA conveyor id
	if (nextCell->conveyorId != INVALID_CONVEYOR)
	{
		ConveyorId nextId = nextCell->conveyorId;

		// Try insert into next conveyor slot 0
		if (InsertItem(nextId, Item{t}))
		{
			// Remove last slot
			mSlotType[lastIdx] = ItemType::NONE;
		}
		return;
	}

	// Case B: Next cell has a normal GameObject (Crafter)
	if (nextCell->gameObject)
	{
		// Check that is a crafter
		if (nextCell->gameObject->type == GameObject::ObjectType::CRAFTER)
		{
			Crafter* c = static_cast<Crafter*>(nextCell->gameObject);

			// Try insert the item in the crafter
			if (c->InsertInput(Item{t}))
			{
				mSlotType[lastIdx] = ItemType::NONE;
			}
		}
	}
}
