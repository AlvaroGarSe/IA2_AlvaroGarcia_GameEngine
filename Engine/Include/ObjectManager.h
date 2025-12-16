#pragma once
#include "Object.h"
//#include "LTexture.h"
#include <vector>
#include "Singleton.h"

/*****************************************************************************/
//I tried to create an Obejct manager that would contain all the textures and objects and at the end close all of them with this manager
/*****************************************************************************/


class ObjectManager:public Singleton<ObjectManager>
{
	/*****************************************************************************/
	friend class Singleton<ObjectManager>;
	/*****************************************************************************/

private:

	//Contains the objects
	std::vector<Object*> objects;

	//Contains all the textures
	//std::vector<LTexture*> textures;

	ObjectManager() { objects = std::vector<Object*>(); /*textures = std::vector<LTexture*>(); */ };
	
public:

		Object* GetObject(int idex);
		Object& GetObject2(int idex) { return *objects[idex]; };

		//LTexture* GetTexture(int idex);
	
		void AddObject(Object* newObject);

		//void AddTexture(LTexture* newTexture);
		
		void Update();

		//Close all the Textures
		//void FreeTextures();

		//Close all the Objects
		void FreeObjects();

		void RemoveObject(Object *removeObject);
};