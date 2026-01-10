#pragma once
#include "Conveyor.h"
#include "IConveyorSystem.h"
#include <vector>

class AoSConveyorSystem : public IConveyorSystem
{
public:
	~AoSConveyorSystem();

	std::vector<Conveyor*> mConveyors;

	ConveyorId Create(int cX, int cY, GameObject::Orientation o) override;
	void StartAll() override;
	void UpdateAll(uint32_t now) override;
	void RenderAll() override;

	void RotateConveyor(ConveyorId id, int dir) override;

	ConveyorId CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o) override;

	bool CanAcceptItem(ConveyorId id) const override;
	bool InsertItem(ConveyorId id, const Item& item) override;
	bool TryExtractItem(ConveyorId id, Item& out) override;
	SDL_Point GetWorldPos(ConveyorId id) const override;
	GameObject::Orientation GetOrientation(ConveyorId id) const override;

	int IndexFromId(ConveyorId id) const { return (int)id - 1; }
};