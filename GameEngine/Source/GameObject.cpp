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

SDL_Point GameObject::GetPosition()
{
	return SDL_Point{ (int)transform.x, (int)transform.y };
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
}

void GameObject::Rotate(int dir)
{
	if (orientation == GameObject::Orientation::NONE) return;
	double aux = transform.rotation + 90 * dir;
	if (aux >= 360) aux -= 360;
	if (aux < 0) aux += 360;
	transform.rotation = aux;

	if (aux < 45 || aux >= 315)
	{
		orientation = GameObject::Orientation::NORTH;
	}
	else if (aux < 135)
	{
		orientation = GameObject::Orientation::EAST;
	}
	else if (aux < 225)
	{
		orientation = GameObject::Orientation::SOUTH;
	}
	else if (aux < 315)
	{
		orientation = GameObject::Orientation::WEST;
	}
}
