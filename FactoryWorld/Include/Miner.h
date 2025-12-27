#pragma once
#include "SDL.h"
#include "LTexture.h"
#include "Object.h"

class Miner :public Object
{
public:

	//Initializes the variables
	Miner(int posX = 0, int posY = 0);

	//Set position of the conveyor
	void setPosition(int posX, int posY);

	void render();

	void startMiner();

	void stopMiner();

	void Update() override;

	void mine();

	void transferMaterial();

	//Loads media
	bool loadMedia();

	LTexture* getMinerTexture() { return gMinerTexture; };

	int maxCapacity;
	int amountResources;

	float minningMaxTime;
	float minningCurrentTime;

	bool isOn;


private:

	//Scene textures
	LTexture* gMinerTexture = new LTexture;
};