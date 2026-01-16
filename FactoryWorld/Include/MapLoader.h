#pragma once
#include "GridCell.h"
#include <string>

#include "json.hpp"
using json = nlohmann::json;

inline TileType TileTypeFromString(const std::string& s)
{
    if (s == "Ground") return TileType::Ground;
    if (s == "IronVein") return TileType::IronVein;
    if (s == "CopperVein") return TileType::CopperVein;
    if (s == "Water") return TileType::Water;
    return TileType::Ground;
}

struct BuildingSaveData
{
    std::string type;
    int x = 0, y = 0;
    GameObject::Orientation orientation = GameObject::Orientation::NONE;
    std::string recipeId; // Only for the Crafter
    bool hasRecipe = false;
};

class MapLoader
{
public:
    // JSON MAP Load the grid of the map and the tiles
    static bool LoadMap(const std::string& path);

    // JSON BUILDINGS Load the buildings and place in the grid
    static bool LoadBuildings(const std::string& path);

    static bool SaveBuildings(const std::string& path);
    static bool SaveBuildingsAsync(const std::string& path); // Multithreading

    static bool IsSaving();

private: 

    static std::vector<BuildingSaveData> CollectBuildingsSnapshot();
    static json BuildBuildingsJson(const std::vector<BuildingSaveData>& snapshot);
    static bool WriteJsonToFileAtomic(const std::string& path, const json& j);

    // Variable to check when using multithreading if it is already saving
    static std::atomic<bool> sIsSaving;
};