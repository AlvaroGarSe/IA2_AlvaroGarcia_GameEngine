#include "Miner.h"
#include "TimeManager.h"
#include "GridManager.h"
#include "Conveyor.h"
#include "ConveyorManager.h"
#include "Item.h"

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

	type = ObjectType::MINER;

	texturePath = "Media/Miner.png";
}

void Miner::Start()
{
	GameObject::Start();

    switch (GridManager::GetInstance().GetTileWithTransform(transform.x, transform.y))
    {
    case TileType::IronVein:
        isOn = true;
        break;
    case TileType::CopperVein:
        isOn = true;
        break;
    default:
        isOn = false;
        return;
        break;
    }
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
    if (!nextCell) return false;

    ConveyorId convId = nextCell->conveyorId;
    if (convId == INVALID_CONVEYOR) return false;

    ItemType type;
    switch (GridManager::GetInstance().GetTileWithTransform(transform.x, transform.y))
    {
    case TileType::IronVein:
        type = ItemType::IronOre;
        break;
    case TileType::CopperVein:
        type = ItemType::CopperOre;
        break;
    default:
        return false;
        break;
    }

    Item ore{ type};
    if (ConveyorManager::GetInstance().InsertItem(convId, ore))
    {
        currentCapacity--;
        return true;
    }
	return false;
}

void Miner::Update(uint32_t now)
{
    if (!isActive || !isOn) return;

    if (now >= minningCurrentTime)
    {
        Mine();
    }
    if (currentCapacity > 0 && now >= nextOutputTime)
    {
        if (TransferMaterial())
        {
			nextOutputTime = now + outputIntervalMs;
        }
    }

}