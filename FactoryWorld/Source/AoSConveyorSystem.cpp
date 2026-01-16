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
		// This is for checking the removed conveyors at runtime
		if (!c) continue;
		c->Update(now);
	}
}

void AoSConveyorSystem::RenderAll()
{
	for (Conveyor* c : mConveyors)
	{
		// This is for checking the removed conveyors at runtime
		if (!c) continue;
		c->Render();
	}
	for (Conveyor* c : mConveyors)
	{
		// This is for checking the removed conveyors at runtime
		if (!c) continue;
		c->RenderItems();
	}
}

void AoSConveyorSystem::RenderAllItemsBehind()
{
	for (Conveyor* c : mConveyors)
	{
		// This is for checking the removed conveyors at runtime
		if (!c) continue;
		c->Render();
		c->RenderItems();
	}
}

void AoSConveyorSystem::RotateConveyor(ConveyorId id, int dir)
{
	mConveyors[IndexFromId(id)]->Rotate(dir);
}

void AoSConveyorSystem::RemoveConveyor(ConveyorId id)
{
	int i = IndexFromId(id);
	if (i < 0 || i >= (int)mConveyors.size()) return;

	if (mConveyors[i])
	{
		delete mConveyors[i];
		mConveyors[i] = nullptr;
	}
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
