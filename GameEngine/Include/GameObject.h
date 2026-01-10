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
		EAST,
		SOUTH,
		WEST
	};

	Transform transform;

	// Shared texture pointer from AssetManager (non-owning)
	LTexture* texture = nullptr;

	// Path to request from AssetManager
	std::string texturePath;

	bool isActive = false;

	Orientation orientation = Orientation::NONE;

	ObjectType type = ObjectType::NONE;

	int mWidth = 0, mHeigth = 0;

	GameObject();
	virtual ~GameObject() = default;

	virtual void Start() { isActive = true; };

	virtual void Update() {};

	virtual void Render();

	void SetPosition(float x, float y);
	SDL_Point GetPosition();

	// -1 rotate -90 || 1 rotate 90
	void Rotate(int dir);

	// Requests texture from AssetManager (no per-instance load)
	bool LoadMedia();

};
