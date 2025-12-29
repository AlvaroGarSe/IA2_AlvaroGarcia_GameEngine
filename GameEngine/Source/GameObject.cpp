#include "GameObject.h"
#include "GraphicManager.h"

GameObject::GameObject()
{
	//Initialize the offsets
	transform.x = 0;
	transform.y = 0;
	transform.scaleX = 1;
	transform.scaleY = 1;
	transform.rotation = 0;

	orientation = Orientation::NONE;
	type = ObjectType::NONE;
}

void GameObject::Update()
{
	
}

void GameObject::Render()
{
	if (!isActive || texture == nullptr)
	{
		printf("Failed to render object");
		return;
	}

	GraphicManager::GetInstance().DrawTexture(texture, transform);
}

void GameObject::SetPosition(float x, float y)
{
	transform.x = x;
	transform.y = y;
}

void GameObject::free()
{

}

bool GameObject::LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Conveyor texture
	if (!texture->loadFromFile(texture->mTexturePath))
	{
		printf("Failed to load {} texture!\n", texture->mTexturePath);
		success = false;
	}
	else
	{
		mWidth = texture->getWidth();
		mHeigth = texture->getHeight();
	}

	return success;
}
