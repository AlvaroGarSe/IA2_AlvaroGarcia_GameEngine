#pragma once
#include "GameObject.h"
//#include "LTexture.h"
#include <vector>
#include "Singleton.h"

/*****************************************************************************/
//I tried to create an Obejct manager that would contain all the textures and
// s and at the end close all of them with this manager
/*****************************************************************************/


class ObjectManager:public Singleton<ObjectManager>
{
	/*****************************************************************************/
	friend class Singleton<ObjectManager>;
	/*****************************************************************************/

private:

	//Contains the objects
	std::vector<GameObject*> objects;

	//Contains all the textures
	//std::vector<LTexture*> textures;

	ObjectManager() { objects = std::vector<GameObject*>(); /*textures = std::vector<LTexture*>(); */ };
	
public:

		GameObject* GetObject(int idex) const;
		GameObject& GetObject2(int idex) { return *objects[idex]; };

		//LTexture* GetTexture(int idex);
	
		void AddObject(GameObject* newObject);

		//void AddTexture(LTexture* newTexture);
		
		void Update();

		//Close all the Textures
		//void FreeTextures();

		//Close all the Objects
		void FreeObjects();

		void RemoveObject(GameObject* removeObject);
};