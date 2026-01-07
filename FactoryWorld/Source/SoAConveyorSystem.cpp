#include "SoAConveyorSystem.h"
#include "TimeManager.h"
#include "GridManager.h"
ConveyorId SoAConveyorSystem::Create(float x, float y, GameObject::Orientation o)
{
	mX.push_back(x);
	mY.push_back(y);
	mOri.push_back(o);
	
	mNextMoveTime.push_back(0);
	mActive.push_back(false);
	
	// Make the slots empty of the conveyor
	for (int i = 0; i < SLOT_COUNT; i++)
	    mSlotType.push_back(ItemType::NONE);

	// id = index+1
	ConveyorId id = mX.size();

	if (GridCell* cell = GridManager::GetInstance().GetCellWorldPos(x, y))
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
	for (size_t i = 0; i < mX.size(); ++i)
		{
		    if (!mActive[i]) continue;
		
		    if (now < mNextMoveTime[i]) continue;
		    mNextMoveTime[i] = now + mMoveIntervalMs;
		
		    // PassItemNextCell(i) ...
		    // MoveItemThroughSlots(i) ...
		}
}

void SoAConveyorSystem::RenderAll()
{
	// draw belt sprite (use x,y,orientation)
//    // draw items using slots
}

bool SoAConveyorSystem::CanAcceptItem(ConveyorId id) const
{
	int i = (int)id - 1;
	    if (i < 0 || i >= (int)mX.size()) return false;
	
	    return SlotEmpty(i, 0);
}

bool SoAConveyorSystem::InsertItem(ConveyorId id, const Item& item)
{
	int i = (int)id - 1;
	if (i < 0 || i >= (int)mX.size()) return false;
	if (!SlotEmpty(i, 0)) return false;
	
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