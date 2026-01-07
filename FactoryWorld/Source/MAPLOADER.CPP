#include "MapLoader.h"
#include <fstream>
#include <iostream>

#include "json.hpp"
using json = nlohmann::json;

#include "GridManager.h"
#include "AssetManager.h"


bool MapLoader::Load(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open())
    {
        std::cout << "MapLoader: cannot open " << path << "\n";
        return false;
    }

    json j;
    try { f >> j; }
    catch (const std::exception& e)
    {
        std::cout << "MapLoader: JSON parse error: " << e.what() << "\n";
        return false;
    }

    int w = j.value("width", 0);
    int h = j.value("height", 0);
    int cellSize = j.value("cellSize", 0);

    if (w <= 0 || h <= 0 || cellSize <= 0)
    {
        std::cout << "MapLoader: invalid width/height/cellSize\n";
        return false;
    }

    GridManager& grid = GridManager::GetInstance();
    grid.CreateGrid(w, h, cellSize);

    // 1) Fill everything with default tile
    TileType def = TileTypeFromString(j.value("defaultTile", "Ground"));
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            grid.SetTile(x, y, def);

    // 2) If flat tiles exist, load them and we're done (no areas/cells needed)
    if (j.contains("tiles") && j["tiles"].is_array())
    {
        const auto& tiles = j["tiles"];
        if ((int)tiles.size() != w * h)
        {
            std::cout << "MapLoader: tiles size mismatch. expected "
                << (w * h) << " got " << tiles.size() << "\n";
            return false;
        }

        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                int idx = y * w + x;
                const std::string tname = tiles[idx].get<std::string>();
                grid.SetTile(x, y, TileTypeFromString(tname));
            }
        }
    }
    else
    {
        // 3) Sparse mode: apply areas
        if (j.contains("areas") && j["areas"].is_array())
        {
            for (const auto& a : j["areas"])
            {
                TileType t = TileTypeFromString(a.value("tile", "Ground"));

                auto from = a["from"]; // [x,y]
                auto to = a["to"];   // [x,y]

                int x0 = from[0].get<int>();
                int y0 = from[1].get<int>();
                int x1 = to[0].get<int>();
                int y1 = to[1].get<int>();

                if (x0 > x1) std::swap(x0, x1);
                if (y0 > y1) std::swap(y0, y1);

                for (int y = y0; y <= y1; ++y)
                    for (int x = x0; x <= x1; ++x)
                        if (grid.IsInside(x, y))
                            grid.SetTile(x, y, t);
            }
        }

        // 4) Sparse mode: apply individual cells (last = overrides)
        if (j.contains("cells") && j["cells"].is_array())
        {
            for (const auto& c : j["cells"])
            {
                int x = c.value("x", 0);
                int y = c.value("y", 0);
                TileType t = TileTypeFromString(c.value("tile", "Ground"));

                if (grid.IsInside(x, y))
                    grid.SetTile(x, y, t);
            }
        }
    }

    // Optional preload
    if (j.value("preloadTileTextures", true))
    {
        AssetManager& assets = AssetManager::GetInstance();
        assets.GetTexture("Media/Tiles/Ground.png");
        assets.GetTexture("Media/Tiles/IronVein.png");
        assets.GetTexture("Media/Tiles/CooperVein.png");
        assets.GetTexture("Media/Tiles/Water.png");
    }

    std::cout << "MapLoader: Map loaded successfully\n";

    return true;
}

