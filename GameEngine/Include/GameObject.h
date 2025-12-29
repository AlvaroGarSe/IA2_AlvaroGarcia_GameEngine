#pragma once
#include "LTexture.h"
#include "Transform.h"

class GameObject
{
public:

	enum class ObjectType : uint8_t
	{
		NONE,
		MINER,
		CONVEYOR,
		CRAFTER,
	};

	enum class Orientation : uint8_t
	{
		NONE,
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	Transform transform;
	LTexture* texture;

	GameObject();

	virtual void Start() { isActive = true; };

	virtual void Update();

	virtual void Render();

	void SetPosition(float x, float y);

	void free();

	bool LoadMedia();

	int mWidth, mHeigth;

	bool isActive;

	Orientation orientation;

	ObjectType type;

	//LTexture* getTexture() { return texture; };
};
