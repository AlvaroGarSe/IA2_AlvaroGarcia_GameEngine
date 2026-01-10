#include "PlayerControlls.h"
#include "GridManager.h"
#include "ConveyorManager.h"
#include "ObjectManager.h"
#include "Miner.h"
#include "Crafter.h"

void PlayerControlls::CreateMiner(int cx, int cy)
{
	GameObject* miner = ObjectManager::GetInstance().Spawn<Miner>(GameObject::Orientation::NORTH);
	if (GridManager::GetInstance().PlaceObject(miner, cx, cy))
	{
		miner->LoadMedia();
		miner->Start();
		return;
	}
}

void PlayerControlls::CreateConveyor(int cx, int cy)
{
	ConveyorManager::GetInstance().CreateConveyorRuntime(cx, cy, GameObject::Orientation::NORTH);
}

void PlayerControlls::CreateCrafter(int cx, int cy)
{
	GameObject* crafter = ObjectManager::GetInstance().Spawn<Crafter>(GameObject::Orientation::NORTH);
	if (GridManager::GetInstance().PlaceObject(crafter, cx, cy))
	{
		crafter->LoadMedia();
		crafter->Start();
		return;
	}
}

void PlayerControlls::RotateBuilding(int cx, int cy, int dir)
{
	GridCell* cell = GridManager::GetInstance().GetCell(cx, cy);
	if (!cell) return;
	if (cell->conveyorId != INVALID_CONVEYOR)
	{
		ConveyorManager::GetInstance().RotateConveyor(cell->conveyorId, dir);
	}
	else if (cell->gameObject != nullptr)
	{
		cell->gameObject->Rotate(dir);
	}
}
