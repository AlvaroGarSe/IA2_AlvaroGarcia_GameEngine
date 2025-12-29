#pragma once
#include "SDL.h"
#include "GameObject.h"

class Player :public GameObject
{
public:

	//Initializes the variables
	Player(int posX = 0, int posY = 0);

};