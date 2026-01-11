#include "MapLoader.h"
#include <fstream>
#include <iostream>

#include "GridManager.h"
#include "AssetManager.h"
#include "ConveyorManager.h"
#include "Crafter.h"
#include "Miner.h"
#include "ObjectManager.h"


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

    //LoadBuildings(j);

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

static GameObject::Orientation OrientationFromString(const std::string& s)
{
    if (s == "NORTH") return GameObject::Orientation::NORTH;
    if (s == "SOUTH") return GameObject::Orientation::SOUTH;
    if (s == "EAST")  return GameObject::Orientation::EAST;
    if (s == "WEST")  return GameObject::Orientation::WEST;
    return GameObject::Orientation::NONE;
}

static bool IsInsideGrid(int x, int y)
{
    auto& g = GridManager::GetInstance();
    return g.IsInside(x, y);
}

bool MapLoader::LoadBuildings(const std::string& path)
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

    if (!j.contains("buildings") || !j["buildings"].is_array()){
        printf("NO OBJ");
        return true; // no buildings = not an error
    }

    auto& grid = GridManager::GetInstance();

    for (const auto& b : j["buildings"])
    {
        if (!b.is_object()) continue;

        const std::string type = b.value("type", "");
        const int x = b.value("x", -1);
        const int y = b.value("y", -1);
        const std::string oStr = b.value("orientation", "NONE");
        const GameObject::Orientation o = OrientationFromString(oStr);

        if (!IsInsideGrid(x, y))
            continue;

        // Prevent double placement if something already there
        GridCell* cell = grid.GetCell(x, y);
        if (!cell) continue;
        if (cell->gameObject || cell->conveyorId != INVALID_CONVEYOR) continue;

        // ---- Conveyor (special because can be AoS or SoA)
        if (type == "Conveyor")
        {
            // If you're in SoA mode, use ConveyorManager (recommended)
            // If you're in AoS mode, Spawn<Conveyor> and place it

            // Easiest: always go through ConveyorManager if you want the system to decide:
            ConveyorManager::GetInstance().Create(x, y, o);
            continue;
        }

        // ---- Miner
        if (type == "Miner")
        {
            auto* m = ObjectManager::GetInstance().Spawn<Miner>(o);
            grid.PlaceObject(m, x, y);
            continue;
        }

        // ---- Crafter
        if (type == "Crafter")
        {
            std::string recipeId = b.value("recipeId", "iron_ingot");
            auto* c = ObjectManager::GetInstance().Spawn<Crafter>(o, recipeId);
            grid.PlaceObject(c, x, y);
            continue;
        }

        // Unknown type -> ignore
    }

    return true;
}

static const char* OrientationToString(GameObject::Orientation o)
{
    switch (o)
    {
    case GameObject::Orientation::NORTH: return "NORTH";
    case GameObject::Orientation::SOUTH: return "SOUTH";
    case GameObject::Orientation::EAST:  return "EAST";
    case GameObject::Orientation::WEST:  return "WEST";
    default: return "NONE";
    }
}

static const char* ObjectTypeToString(GameObject::ObjectType t)
{
    switch (t)
    {
    case GameObject::ObjectType::MINER:    return "Miner";
    case GameObject::ObjectType::CRAFTER:  return "Crafter";
    case GameObject::ObjectType::CONVEYOR: return "Conveyor";
    default: return "Unknown";
    }
}

bool MapLoader::SaveBuildings(const std::string& path)
{
    GridManager& grid = GridManager::GetInstance();

    const int w = grid.GetWidth();
    const int h = grid.GetHeight();

    if (w <= 0 || h <= 0)
    {
        std::cout << "MapLoader::SaveBuildings: grid not created\n";
        return false;
    }

    json out;
    out["buildings"] = json::array();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            GridCell* cell = grid.GetCell(x, y);
            if (!cell) continue;

            // --- Case 1: SoA conveyor stored as conveyorId (no GameObject)
            if (cell->conveyorId != 0)
            {
                json b;
                b["type"] = "Conveyor";
                b["x"] = x;
                b["y"] = y;

                // Get orientation from the system/manager if you have it
                // If you don't yet, you can skip this field.
                b["orientation"] = OrientationToString(ConveyorManager::GetInstance().GetOrientation(cell->conveyorId));

                out["buildings"].push_back(b);
                continue;
            }

            // --- Case 2: Classic GameObject building
            if (!cell->gameObject) continue;

            GameObject* go = cell->gameObject;

            // Only save buildings you care about
            if (go->type != GameObject::ObjectType::MINER &&
                go->type != GameObject::ObjectType::CRAFTER 
                //&&
                //go->type != GameObject::ObjectType::CONVEYOR
                )
                continue;

            json b;
            b["type"] = ObjectTypeToString(go->type);
            b["x"] = x;
            b["y"] = y;
            b["orientation"] = OrientationToString(go->orientation);

            // Crafter extra data
            if (go->type == GameObject::ObjectType::CRAFTER)
            {
                Crafter* c = static_cast<Crafter*>(go);

                b["recipeId"] = c->GetRecipeId();
            }

            out["buildings"].push_back(b);
        }
    }

    try
    {
        std::ofstream f(path);
        if (!f.is_open())
        {
            std::cout << "MapLoader::SaveBuildings: cannot open " << path << "\n";
            return false;
        }
        f << out.dump(2); // pretty print
    }
    catch (const std::exception& e)
    {
        std::cout << "MapLoader::SaveBuildings: write error: " << e.what() << "\n";
        return false;
    }

    std::cout << "MapLoader::SaveBuildings: saved to " << path << "\n";
    return true;
}

