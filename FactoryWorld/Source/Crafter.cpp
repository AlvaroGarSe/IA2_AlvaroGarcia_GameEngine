#include "Crafter.h"
#include "TimeManager.h"
#include "GridManager.h"
#include "Conveyor.h"
#include "item.h"

Crafter::Crafter(Orientation orientation)
{
    SetPosition(0, 0);

    craftingMaxTime = 5000;
    craftingCurrentTime = 0;

	transform.scaleX = 0.5;
	transform.scaleY = 0.5;

	this->orientation = orientation;

    switch (orientation)
    {
    case GameObject::Orientation::NONE:
        transform.rotation = 0;
        break;
    case GameObject::Orientation::NORTH:
        transform.rotation = 0;
        break;
    case GameObject::Orientation::SOUTH:
        transform.rotation = 180;
        break;
    case GameObject::Orientation::EAST:
        transform.rotation = 90;
        break;
    case GameObject::Orientation::WEST:
        transform.rotation = 270;
        break;
    default:
        break;
    }


    isActive = false;
	isCrafting = false;

    isOn = false;

    texture = new LTexture;

	type = ObjectType::CRAFTER;

	texture->mTexturePath = "Media/Crafter.png";
}

Crafter::~Crafter()
{
    for (Item* item : mInputSlots) delete item;
	delete mOutputItem;
}

void Crafter::Start()
{
	GameObject::Start();
    isOn = true;
}

void Crafter::StopCrafter()
{
    isOn = false;
	isCrafting = false;
}



bool Crafter::CanAcceptInput() const
{
    for (Item* slot: mInputSlots)
    {
        if (!slot)
        {
            return true;
        }
    }
    return false;
}

bool Crafter::InsertInput(const Item& item)
{
    for (Item*& slot : mInputSlots)
    {
        if (!slot)
        {
			slot = new Item(item);
			TryStartCraft();
            return true;
        }
    }
    return false;
}

bool Crafter::CanOutput() const
{
    return mOutputItem != nullptr;
}

void Crafter::TryStartCraft()
{
	if (isCrafting || mOutputItem) return;
	
    if (mInputSlots[0] && mInputSlots[1] &&
        mInputSlots[0]->type == ItemType::IronOre && mInputSlots[1]->type == ItemType::IronOre)
    {
	    isCrafting = true;
        craftingCurrentTime = TimeManager::GetInstance().getTicks() + craftingMaxTime;
    }
}

void Crafter::FinishCraft()
{
	isCrafting = false;

	delete mInputSlots[0];
	mInputSlots[0] = nullptr;
	delete mInputSlots[1];
	mInputSlots[1] = nullptr;

	mOutputItem = new Item{ ItemType::IronIngot };
}

void Crafter::Update()
{
    if (!isActive || !isOn) return;

    if (isCrafting && TimeManager::GetInstance().getTicks() >= craftingCurrentTime)
    {
		FinishCraft();
    }

    if (CanOutput())
    {
        GridManager& gridManager = GridManager::GetInstance();

        SDL_Point posGrid = gridManager.WorldToGrid((int)transform.x, (int)transform.y);

        GridCell* nextCell = gridManager.GetAdjacentCell(posGrid.x, posGrid.y, orientation);

        // Checks if the cell is valid and has an item object in it
        if (!nextCell || !nextCell->gameObject) return;

        if (nextCell->gameObject->type == ObjectType::CONVEYOR)
        {
            Conveyor* nextConv = static_cast<Conveyor*>(nextCell->gameObject);

            if (nextConv->CanAcceptItem())
            {
                if (nextConv->InsertItem(*mOutputItem))
                {
                    delete mOutputItem;
                    mOutputItem = nullptr;
                }
            }
        }
    }
}