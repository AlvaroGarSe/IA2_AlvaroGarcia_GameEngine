#pragma once
#include "SDL.h"
#include "GameObject.h"

class Miner :public GameObject
{
public:

	//Initializes the variables
	Miner(Orientation orientation = Orientation::NORTH);

	void Start() override;

	void StopMiner();

	void Update() override;

	void Mine();

	bool TransferMaterial();

private:

	int maxCapacity;
	int currentCapacity;

	Uint32 minningMaxTime;
	Uint32 minningCurrentTime;

	Uint32 outputIntervalMs = 300;
	Uint32 nextOutputTime = 0;

	bool isOn;
};