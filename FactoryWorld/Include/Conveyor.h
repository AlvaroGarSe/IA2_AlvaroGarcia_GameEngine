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

	void Update(uint32_t now) override;
	void RenderItems();

	bool CanAcceptItem() const;
	// Inserts an item into slot 0
	bool InsertItem(const Item& item); 

	// Extracts an item from the last slot
	bool TryExtractItem(Item& outItem);

private:

	void MoveItemThroughSlots();
	void PassItemNextCell();

	// Stores the items inside the conveyor
	std::array<Item*, SLOT_COUNT> mSlots{};

	// Speed of the conveyor
	uint32_t mMoveIntervalMs = 300;
	uint32_t mNextMoveTime = 0;
};