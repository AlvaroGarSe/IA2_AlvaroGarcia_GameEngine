#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Singleton.h"
#include "LTexture.h"

class AssetManager : public Singleton<AssetManager>
{
	/*****************************************************************************/
	friend class Singleton<AssetManager>;
	/*****************************************************************************/

private:

	AssetManager() {};

	// key: "Media/Conveyor.png"  value: unique texture loaded once
	std::unordered_map<std::string, std::unique_ptr<LTexture>> mTextures;

public:

	// Returns a shared raw pointer (non-owning). AssetManager keeps ownership.
	LTexture* GetTexture(const std::string& path);

	void Clear(); // Frees all textures (called on shutdown)

};