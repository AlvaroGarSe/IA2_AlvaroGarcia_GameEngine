#pragma once
#include "SDL.h"
#include "GameObject.h"

class Miner :public GameObject
{
public:

	//Initializes the variables
	Miner(int posX = 0, int posY = 0);

	void startMiner();

	void stopMiner();

	void Update() override;

	void mine();

	void transferMaterial();

	//Loads media
	bool loadMedia();

	int maxCapacity;
	int amountResources;

	float minningMaxTime;
	float minningCurrentTime;

	bool isOn;
};