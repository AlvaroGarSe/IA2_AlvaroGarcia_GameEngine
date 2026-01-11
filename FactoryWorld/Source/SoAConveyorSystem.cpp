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
	ConveyorId id = mX.size();

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
	for (int i = 0; i < (int)mX.size(); ++i)
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
	case ItemType::IronOre:      return "Media/Items/IronOre.png";
	case ItemType::IronIngot:    return "Media/Items/IronIngot.png";
	case ItemType::CopperOre:    return "Media/Items/CopperOre.png";
	case ItemType::CopperIngot:  return "Media/Items/CopperIngot.png";
	default:                     return "Media/Items/Unknown.png";
	}
}

void SoAConveyorSystem::RenderAll()
{
	auto& gfx = GraphicManager::GetInstance();

	const int cellSize = GridManager::GetInstance().GetCellSize();

	const int slotSpacingWorld = cellSize / SLOT_COUNT;

	// World-space sprite size
	const float itemSizeWorld = 32.0f;

	Transform ct;
	ct.scaleX = 0.5;
	ct.scaleY = 0.5;

	// Renders first all the Conveyor sprites to not render some items behind the next conveyor
	for (int i = 0; i < (int)mX.size(); ++i)
	{
		if (!mActive[i]) continue;

		// Direction of the sprite deppending on the orientation
		int dirX = 0, dirY = 0;
		switch (mOri[i])
		{
		case GameObject::Orientation::NORTH: dirX = 0;  dirY = -1; ct.rotation = 0; break;
		case GameObject::Orientation::SOUTH: dirX = 0;  dirY = 1; ct.rotation = 180; break;
		case GameObject::Orientation::EAST:  dirX = 1;  dirY = 0; ct.rotation = 90;  break;
		case GameObject::Orientation::WEST:  dirX = -1; dirY = 0; ct.rotation = 270; break;
		default: ct.rotation = 0; break;
		}

		ct.x = mX[i];
		ct.y = mY[i];

		GraphicManager::GetInstance().DrawTexture(convTexture, ct);

	}

	for (int i = 0; i < (int)mX.size(); ++i)
	{
		if (!mActive[i]) continue;

		// Direction of the sprite deppending on the orientation
		int dirX = 0, dirY = 0;
		switch (mOri[i])
		{
		case GameObject::Orientation::NORTH: dirX = 0;  dirY = -1; ct.rotation = 0; break;
		case GameObject::Orientation::SOUTH: dirX = 0;  dirY = 1; ct.rotation = 180; break;
		case GameObject::Orientation::EAST:  dirX = 1;  dirY = 0; ct.rotation = 90;  break;
		case GameObject::Orientation::WEST:  dirX = -1; dirY = 0; ct.rotation = 270; break;
		default: ct.rotation = 0; break;
		}

		//GraphicManager::GetInstance().DrawTexture(convText, ct);

		// but then used itemSizeWorld=32 for the sprite. We'll mimic your logic.)
		const float centerItemForCell = 12.0f;
		const float centerOffsetWorld = (cellSize - centerItemForCell) / 2.0f;

		float startX = mX[i] + centerOffsetWorld;
		float startY = mY[i] + centerOffsetWorld;

		for (int s = 0; s < SLOT_COUNT; ++s)
		{
			ItemType t = mSlotType[SlotToIndex(i, s)];
			if (t == ItemType::NONE) continue;

			LTexture* itemTex = AssetManager::GetInstance().GetTexture(ItemSpritePath(t));
			if (!itemTex) continue;

			

			float sx = itemSizeWorld / (float)itemTex->getWidth();
			float sy = itemSizeWorld / (float)itemTex->getHeight();

			float offset = (s * slotSpacingWorld) - (cellSize / 2.0f - slotSpacingWorld / 2.0f);

			Transform it;
			it.x = startX + dirX * offset - itemSizeWorld * 0.25f;
			it.y = startY + dirY * offset - itemSizeWorld * 0.25f;
			it.rotation = 0.0f;
			it.scaleX = sx;
			it.scaleY = sy;

			gfx.DrawTexture(itemTex, it);
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
	    if (i < 0 || i >= (int)mX.size()) return false;
	
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

bool SoAConveyorSystem::TryExtractItem(ConveyorId id, Item& out)
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mX.size()) return false;

	int last = SLOT_COUNT - 1;
	ItemType t = mSlotType[i * SLOT_COUNT + last];
	if (t == ItemType::NONE) return false;

	out.type = t;
	mSlotType[i * SLOT_COUNT + last] = ItemType::NONE;
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
	if (i < 0 || i >= (int)mX.size()) return;

	for (int s = 0; s < SLOT_COUNT; ++s)
		mSlotType[SlotToIndex(i, s)] = ItemType::NONE;

	mActive[i] = false;
	mNextMoveTime[i] = 0;
}

SDL_Point SoAConveyorSystem::GetWorldPos(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mX.size()) return SDL_Point{ 0,0 };
	return SDL_Point{ (int)mX[i], (int)mY[i] };
}

GameObject::Orientation SoAConveyorSystem::GetOrientation(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mOri.size()) return GameObject::Orientation::NONE;
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

	SDL_Point gridPos = grid.WorldToGrid((int)mX[i], (int)mY[i]);
	GridCell* nextCell = grid.GetAdjacentCell(gridPos.x, gridPos.y, mOri[i]);

	if (!nextCell) return;

	// Case A: next cell has a SoA conveyor id
	if (nextCell->conveyorId != 0)
	{
		ConveyorId nextId = nextCell->conveyorId;

		// Try insert into next conveyor slot 0
		if (CanAcceptItem(nextId))
		{
			Item insItem{ t };
			if (InsertItem(nextId, insItem))
			{
				// Remove last slot
				mSlotType[lastIdx] = ItemType::NONE;
			}
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
			if (c->CanAcceptInput())
			{
				Item insItem{ t };
				if (c->InsertInput(insItem))
				{
					mSlotType[lastIdx] = ItemType::NONE;
				}
			}
		}
	}
}
