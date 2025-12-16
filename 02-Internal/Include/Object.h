#pragma once

class Object
{
private:
	
public:
	Object();

	void Update();

	void free();

	int mPosX, mPosY;
	int mWidth, mHeigth;

	bool isActive;
};
