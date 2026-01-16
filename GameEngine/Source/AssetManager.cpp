#include "AssetManager.h"
#include <cstdio>

LTexture* AssetManager::GetTexture(const std::string& path)
{
    auto it = mTextures.find(path);
    if (it != mTextures.end())
        return it->second.get();

    // Not loaded yet -> load once
    auto tex = std::make_unique<LTexture>();

    // LTexture::loadFromFile uses GraphicManager renderer internally
    if (!tex->loadFromFile(path))
    {
        std::printf("AssetManager: Failed to load texture: %s\n", path.c_str());
        return nullptr;
    }

    LTexture* raw = tex.get();
    mTextures.emplace(path, std::move(tex));
    return raw;
}

void AssetManager::Clear()
{
    // unique_ptr frees textures automatically
    mTextures.clear();
}
