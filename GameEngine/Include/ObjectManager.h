#pragma once
#include <memory>
#include <vector>
#include <algorithm>

#include "Singleton.h"
#include "GameObject.h"

// *********************************************IMPORTANT********************************************* //
// The conveyors are not stored here because there will be stored in the AoS/SoAConveyorSystem cause of the AoS and SoA variation
// Any other object that will also be implemented with AoS and SoA will not be stored and managed here


class ObjectManager:public Singleton<ObjectManager>
{
	/*****************************************************************************/
	friend class Singleton<ObjectManager>;
	/*****************************************************************************/

private:

	ObjectManager() {};

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

	void UpdateAll(uint32_t now);

	void LoadAllMedia();

	void RenderAll();

	void Destroy(GameObject* objPtr);

	void Clear();

	size_t Count() const { return objects.size(); }
};