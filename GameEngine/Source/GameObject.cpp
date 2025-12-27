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
}

void GameObject::Update()
{
	
}

void GameObject::render()
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