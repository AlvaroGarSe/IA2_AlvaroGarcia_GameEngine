#include "AoSConveyorSystem.h"
#include "GridManager.h"

AoSConveyorSystem::~AoSConveyorSystem()
{
	for (Conveyor* c : mConveyors) delete c;
	mConveyors.clear();
}

ConveyorId AoSConveyorSystem::Create(int cX, int cY, GameObject::Orientation o)
{
	Conveyor* c = new Conveyor(o);
	GridManager& gridManager = GridManager::GetInstance();

	SDL_Point convPos = gridManager.GridToWorld(cX, cY);

	c->SetPosition(convPos.x, convPos.y);

	mConveyors.push_back(c);

	// id = index+1
	int id = mConveyors.size();

	//gridManager.PlaceObject(c, cX, cY);
	//SDL_Point cellPos = gridManager.WorldToGrid(x, y);
	//gridManager.PlaceObject(c, cellPos.x, cellPos.y);

	//GridCell* cell = gridManager.GetCellWorldPos(x, y);
	GridCell* cell = gridManager.GetCell(cX, cY);
	cell->conveyorId = id;


	return (ConveyorId) id;
}

void AoSConveyorSystem::StartAll()
{
	for (Conveyor* c : mConveyors)
	{
		c->LoadMedia();
		c->Start();
	}
}

void AoSConveyorSystem::UpdateAll(uint32_t now)
{
	for (Conveyor* c : mConveyors)
	{
		c->Update();
	}
}

void AoSConveyorSystem::RenderAll()
{
	for (Conveyor* c : mConveyors)
	{
		c->Render();
	}
}

void AoSConveyorSystem::RotateConveyor(ConveyorId id, int dir)
{
	mConveyors[IndexFromId(id)]->Rotate(dir);
}

ConveyorId AoSConveyorSystem::CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o)
{
	if (!GridManager::GetInstance().CanPlace(cX, cY)) return INVALID_CONVEYOR;
	int id = IndexFromId(Create(cX, cY, o));
	mConveyors[id]->LoadMedia();
	mConveyors[id]->Start();
	return id;
}

bool AoSConveyorSystem::CanAcceptItem(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mConveyors.size()) return false;
	Conveyor* c = mConveyors[i];
	if (!c) return false;
	return c->CanAcceptItem();
}

bool AoSConveyorSystem::InsertItem(ConveyorId id, const Item& item)
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mConveyors.size()) return false;
	Conveyor* c = mConveyors[i];
	if (!c) return false;
	return c->InsertItem(item);
}

bool AoSConveyorSystem::TryExtractItem(ConveyorId id, Item& outItem)
{
	int i = id - 1;

	if (i < 0 || i >= (int)mConveyors.size()) return false;

	if (!mConveyors[i]) return false;

	return mConveyors[i]->TryExtractItem(outItem);
}

SDL_Point AoSConveyorSystem::GetWorldPos(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mConveyors.size()) return SDL_Point{ 0,0 };
	Conveyor* c = mConveyors[i];
	if (!c) return SDL_Point{ 0,0 };

	return c->GetPosition();
}

GameObject::Orientation AoSConveyorSystem::GetOrientation(ConveyorId id) const
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mConveyors.size()) return GameObject::Orientation::NONE;

	Conveyor* c = mConveyors[i];
	if (!c) return GameObject::Orientation::NONE;

	return c->orientation;
}
