#pragma once
#include "GameObject.h"
#include "item.h"
#include <array>

class Conveyor :public GameObject
{
public:

	static constexpr int SLOT_COUNT = 4;

	//Initializes the variables
	Conveyor(Orientation orientation = Orientation::NORTH);
	~Conveyor();

	void Update() override;
	void Render() override;

	bool CanAcceptItem() const;
	bool InsertItem(const Item& item); // Inserts an item into slot 0

private:

	bool TryExtractItem(Item& outItem); // Extracts an item from the last slot
	void MoveItemThroughSlots();
	void PassItemNextCell();

	std::array<Item*, SLOT_COUNT> mSlots{};

	uint32_t mMoveIntervalMs = 300; // e.g. 0.3s per step
	uint32_t mNextMoveTime = 0;

};