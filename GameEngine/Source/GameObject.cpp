#include "GameObject.h"
#include "GraphicManager.h"
#include "AssetManager.h"
#include <cstdio>

GameObject::GameObject()
{
	//Initialize the offsets
	transform.x = 0;
	transform.y = 0;
	transform.scaleX = 1;
	transform.scaleY = 1;
	transform.rotation = 0;

}


void GameObject::Render()
{
	if (!isActive || texture == nullptr)
	{
		return;
	}

	GraphicManager::GetInstance().DrawTexture(texture, transform);
}

void GameObject::SetPosition(float x, float y)
{
	transform.x = x;
	transform.y = y;
}


bool GameObject::LoadMedia()
{
	if (texturePath.empty())
	{
		printf("GameObject::LoadMedia - texturePath empty\n");
		return false;
	}

	texture = AssetManager::GetInstance().GetTexture(texturePath);
	if (!texture) return false;

	mWidth = texture->getWidth();
	mHeigth = texture->getHeight();
	return true;


	////Loading success flag
	//bool success = true;

	////Load Conveyor texture
	//if (!texture->loadFromFile(texture->mTexturePath))
	//{
	//	printf("Failed to load {} texture!\n", texture->mTexturePath);
	//	success = false;
	//}
	//else
	//{
	//	mWidth = texture->getWidth();
	//	mHeigth = texture->getHeight();
	//}

	//return success;
}
