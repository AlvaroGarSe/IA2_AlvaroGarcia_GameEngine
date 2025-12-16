#include "ObjectManager.h"

Object* ObjectManager::GetObject(int idex)
{
	return objects[idex];
}

/*
LTexture* ObjectManager::GetTexture(int idex)
{
	return textures[idex];
}*/

void ObjectManager::AddObject(Object* newItem)
{
	objects.push_back(newItem);
}
/*
void ObjectManager::AddTexture(LTexture* newItem)
{
	textures.push_back(newItem);
}*/

void ObjectManager::Update()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}
}
/*
void ObjectManager::FreeTextures()
{
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->free();
	}
}*/

void ObjectManager::FreeObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->free();
	}
	//FreeTextures();
}

void ObjectManager::RemoveObject(Object *removeObject)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == removeObject)
		{
			objects.erase(objects.begin() + i);
		}
	}
}
