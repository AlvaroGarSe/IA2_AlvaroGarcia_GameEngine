#include "ObjectManager.h"

void ObjectManager::StartAll()
{
	for (auto& obj : objects)
	{
		obj->Start();
	}
}

void ObjectManager::UpdateAll(uint32_t now)
{
	for (auto& obj : objects)
	{
		obj->Update(now);
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
	// Destroys and frees one single object
	objects.erase(std::remove_if(objects.begin(), objects.end(),
		[&](const std::unique_ptr<GameObject>& o) { return o.get() == objPtr; }), objects.end());
}

void ObjectManager::Clear()
{
	// Free all the objects because uses unique_ptr
	objects.clear();
}