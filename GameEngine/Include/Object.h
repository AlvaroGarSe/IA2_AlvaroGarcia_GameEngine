#pragma once
#include "LTexture.h"
#include "Transform.h"

class Object
{
public:
	Transform transform;
	LTexture* texture;

	Object();

	virtual void Update();

	virtual void render();

	void free();

	int mWidth, mHeigth;

	bool isActive;

	//LTexture* getTexture() { return texture; };
};
