#include "Miner.h"
#include "TimeManager.h"
#include "GridManager.h"
#include "Conveyor.h"
#include "item.h"

Miner::Miner(Orientation orientation)
{
    SetPosition(0, 0);

    maxCapacity = 10;
    currentCapacity = 0;

    minningMaxTime = 2000;
    minningCurrentTime = 0;

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

    isOn = false;

    texture = new LTexture;

	type = ObjectType::MINER;

	texture->mTexturePath = "Media/Miner.png";
}

void Miner::Start()
{
	GameObject::Start();

    isOn = true;
	Uint32 currentTime = TimeManager::GetInstance().getTicks();

    minningCurrentTime = currentTime + minningMaxTime;
	nextOutputTime = currentTime;
}

void Miner::StopMiner()
{
    isOn = false;
}

void Miner::Mine()
{
    Uint32 currentTime = TimeManager::GetInstance().getTicks();
    if (currentCapacity >= maxCapacity)
    {
        printf("Miner: MaxCapac\n");
        // Don’t spam Mine() every frame when full
        minningCurrentTime = currentTime + 200;
        return;
    }

    currentCapacity++;
    minningCurrentTime = currentTime + minningMaxTime;
}

bool Miner::TransferMaterial()
{
    GridManager& gridManager = GridManager::GetInstance();

    SDL_Point posGrid = gridManager.WorldToGrid((int)transform.x, (int)transform.y);

    GridCell* nextCell = gridManager.GetAdjacentCell(posGrid.x, posGrid.y, orientation);

    // Checks if the cell is valid and has an item object in it
    if (!nextCell || !nextCell->gameObject) return false;

    if (nextCell->gameObject->type == ObjectType::CONVEYOR)
    {
        Conveyor* nextConv = static_cast<Conveyor*>(nextCell->gameObject);
        Item ore{ ItemType::IronOre};
        if (nextConv->InsertItem(ore))
        {
            currentCapacity--;
            return true;
        }
    }
	return false;
}

void Miner::Update()
{
    if (!isActive || !isOn) return;

	Uint32 currentTime = TimeManager::GetInstance().getTicks();

    if (currentTime >= minningCurrentTime)
    {
        Mine();
    }
    if (currentCapacity > 0 && currentTime >= nextOutputTime)
    {
        if (TransferMaterial())
        {
			nextOutputTime = currentTime + outputIntervalMs;
        }
    }

}