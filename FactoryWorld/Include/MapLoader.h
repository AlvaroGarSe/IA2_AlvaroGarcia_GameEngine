#pragma once
#include "GridCell.h"
#include <string>


inline TileType TileTypeFromString(const std::string& s)
{
    if (s == "Ground") return TileType::Ground;
    if (s == "IronVein") return TileType::IronVein;
    if (s == "CopperVein") return TileType::CopperVein;
    if (s == "Water") return TileType::Water;
    return TileType::Ground;
}

class MapLoader
{
public:
    static bool Load(const std::string& path);
};