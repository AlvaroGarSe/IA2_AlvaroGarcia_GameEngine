#pragma once
#include <memory>
#include <vector>
#include <algorithm>

#include "Singleton.h"
#include "GameObject.h"

class ObjectManager:public Singleton<ObjectManager>
{
	/*****************************************************************************/
	friend class Singleton<ObjectManager>;
	/*****************************************************************************/

private:

	ObjectManager() { /*objects = std::vector<GameObject*>(); textures = std::vector<LTexture*>(); */};

	//Contains the objects
	std::vector<std::unique_ptr<GameObject>> objects;
	
public:

	// Generic spawn
	template <typename T, typename... Args>
	T* Spawn(Args&&... args)
	{
		auto obj = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = obj.get();
		objects.push_back(std::move(obj));
		return raw;
	};

	GameObject* GetObject(int index) const { return objects[index].get(); }

	void StartAll();

	void UpdateAll();

	void LoadAllMedia();

	void RenderAll();

	void Destroy(GameObject* objPtr);

	void Clear();

	size_t Count() const { return objects.size(); }





	//GameObject* GetObject(int index) const;
	//GameObject& GetObject2(int index) { return *objects[index]; };

	////LTexture* GetTexture(int idex);
	
	//void AddObject(GameObject* newObject);



	////void AddTexture(LTexture* newTexture);
	//
	//

	////Close all the Textures
	////void FreeTextures();

	////Close all the Objects
	//void FreeObjects();

	//void RemoveObject(GameObject* removeObject);
};