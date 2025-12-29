#pragma once
#include "GameObject.h"
#include <memory>
#include <vector>
#include "Singleton.h"

class ObjectManager:public Singleton<ObjectManager>
{
	/*****************************************************************************/
	friend class Singleton<ObjectManager>;
	/*****************************************************************************/

	template <typename T, typename... Args>
	T* Spawn(Args&&... args)
	{
		auto obj = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = obj.get();
		objects.push_back(std::move(obj));
		return raw;
	}
private:

	//Contains the objects
	std::vector<GameObject*> objects;

	std::vector<std::unique_ptr<GameObject>> objectss;

	//Contains all the textures
	//std::vector<LTexture*> textures;

	ObjectManager() { objects = std::vector<GameObject*>(); /*textures = std::vector<LTexture*>(); */ };
	
public:

		GameObject* GetObject(int index) const;
		GameObject& GetObject2(int index) { return *objects[index]; };

		//LTexture* GetTexture(int idex);
	
		void AddObject(GameObject* newObject);

		void LoadAllMedia();

		void RenderAllObjects();

		//void AddTexture(LTexture* newTexture);
		
		void Start();

		void Update();

		//Close all the Textures
		//void FreeTextures();

		//Close all the Objects
		void FreeObjects();

		void RemoveObject(GameObject* removeObject);
};