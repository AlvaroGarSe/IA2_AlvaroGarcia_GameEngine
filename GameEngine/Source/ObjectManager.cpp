#include "ObjectManager.h"

void ObjectManager::StartAll()
{
	for (auto& obj : objects)
	{
		obj->Start();
	}
}

void ObjectManager::UpdateAll()
{
	for (auto& obj : objects)
	{
		obj->Update();
	}
}

void ObjectManager::LoadAllMedia()
{
	for (auto& obj : objects)
	{
		obj->LoadMedia();
	}
}
void ObjectManager::RenderAll()
{
	for (auto& obj : objects)
	{
		obj->Render();
	}
}

void ObjectManager::Destroy(GameObject* objPtr)
{
	objects.erase(std::remove_if(objects.begin(), objects.end(),
		[&](const std::unique_ptr<GameObject>& o) { return o.get() == objPtr; }),
		objects.end());
}

void ObjectManager::Clear()
{
	objects.clear();
}