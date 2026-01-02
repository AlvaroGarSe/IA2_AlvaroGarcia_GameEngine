#pragma once
#include "SDL.h"
#include "GameObject.h"
#include "item.h"

class Crafter :public GameObject
{
public:
	static constexpr int INPUT_MATERIAL_COUNT = 2;

	//Initializes the variables
	Crafter(Orientation orientation = Orientation::NORTH);
	~Crafter();

	void Start() override;
	void StopCrafter();

	void Update() override;

	// Input
	bool CanAcceptInput() const;
	bool InsertInput(const Item& item);


	// Output
	bool HasOutput() const;

private:

	void TryStartCraft();
	void FinishCraft();

	Item* mInputSlots[INPUT_MATERIAL_COUNT]{};
	Item* mOutputItem = nullptr;

	Uint32 craftingMaxTime;
	Uint32 craftingCurrentTime;

	bool isCrafting;

	bool isOn;
};