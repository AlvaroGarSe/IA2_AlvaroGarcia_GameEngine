#pragma once
#include "LTexture.h"
#include "Transform.h"

class GameObject
{
public:
	Transform transform;
	LTexture* texture;

	GameObject();

	virtual void Update();

	virtual void render();

	void SetPosition(float x, float y);

	void free();

	int mWidth, mHeigth;

	bool isActive;

	//LTexture* getTexture() { return texture; };
};
