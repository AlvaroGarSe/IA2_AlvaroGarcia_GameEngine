#pragma once
#include "GameObject.h"
#include "ConveyorTypes.h"
#include "Item.h"
#include <array>

class Conveyor :public GameObject
{
public:

	//Initializes the variables
	Conveyor(Orientation orientation = Orientation::NORTH);
	~Conveyor();

	void Update() override;
	void RenderItems();
	void RenderItemsBehind();

	bool CanAcceptItem() const;
	bool InsertItem(const Item& item); // Inserts an item into slot 0

	bool TryExtractItem(Item& outItem); // Extracts an item from the last slot

private:

	void MoveItemThroughSlots();
	void PassItemNextCell();

	std::array<Item*, SLOT_COUNT> mSlots{};


	uint32_t mMoveIntervalMs = 300; // e.g. 0.3s per step
	uint32_t mNextMoveTime = 0;

};