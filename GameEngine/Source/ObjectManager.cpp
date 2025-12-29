#include "ObjectManager.h"

GameObject* ObjectManager::GetObject(int index) const
{
	return objects[index];
}

/*
LTexture* ObjectManager::GetTexture(int idex)
{
	return textures[idex];
}*/

void ObjectManager::AddObject(GameObject* newItem)
{
	objects.push_back(newItem);
}
void ObjectManager::LoadAllMedia()
{
	for (GameObject* obj : objects)
	{
		obj->LoadMedia();
	}
}
void ObjectManager::RenderAllObjects()
{
	for (GameObject* obj : objects)
	{
		obj->Render();
	}
}
/*
void ObjectManager::AddTexture(LTexture* newItem)
{
	textures.push_back(newItem);
}*/

void ObjectManager::Start()
{
	for (GameObject* obj : objects)
	{
		obj->Start();
	}
}

void ObjectManager::Update()
{
	for (GameObject* obj : objects)
	{
		obj->Update();
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
	for (GameObject* obj : objects)
	{
		obj->free();
		delete obj;
	}
	//FreeTextures();
}

void ObjectManager::RemoveObject(GameObject* ptr)
{
	/*for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == removeObject)
		{
			objects.erase(objects.begin() + i);
		}
	}*/
	/*for (GameObject* obj : objects)
	{
		if (obj == removeObject)
		{
			objects.erase(std::remove(objects.begin(), objects.end(), removeObject), objects.end());
		}
	}*/

	objects.erase(std::remove_if(objects.begin(), objects.end(), [&](const std::unique_ptr<GameObject>& o) { return o.get() == ptr; }), objects.end());	
}
