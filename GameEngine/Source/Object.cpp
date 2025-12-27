#include "Object.h"
#include "GraphicManager.h"

Object::Object()
{
	//Initialize the offsets
	transform.x = 0;
	transform.y = 0;
	transform.scaleX = 1;
	transform.scaleY = 1;
	transform.rotation = 0;
}

void Object::Update()
{
	
}

void Object::render()
{
	if (!isActive || texture == nullptr)
	{
		printf("Failed to render object");
		return;
	}

	GraphicManager::GetInstance().DrawTexture(texture, transform);
}

void Object::free()
{

}