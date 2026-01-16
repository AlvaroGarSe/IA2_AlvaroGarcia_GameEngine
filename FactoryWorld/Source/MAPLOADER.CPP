#include "MapLoader.h"
#include <fstream>
#include <iostream>

#include "GridManager.h"
#include "AssetManager.h"
#include "ConveyorManager.h"
#include "Crafter.h"
#include "Miner.h"
#include "ObjectManager.h"
#include <thread>

std::atomic<bool> MapLoader::sIsSaving{ false };

bool MapLoader::LoadMap(const std::string& path)
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
            std::cout << "MapLoader: tiles size mismatch. expected " << (w * h) << " got " << tiles.size() << "\n";
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

        // 4) Sparse mode: apply individual cells
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
    
    // Preload textures of the cells
    if (j.value("preloadTileTextures", true))
    {
        AssetManager& assets = AssetManager::GetInstance();
        assets.GetTexture("Media/Tiles/Ground.png");
        assets.GetTexture("Media/Tiles/IronVein.png");
        assets.GetTexture("Media/Tiles/CooperVein.png");
        assets.GetTexture("Media/Tiles/Water.png");
    }

    printf("MapLoader: Map loaded successfully\n");

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
        return true; // no buildings = not an error
    }

    GridManager& grid = GridManager::GetInstance();

    for (const auto& b : j["buildings"])
    {
        if (!b.is_object()) continue;

        const std::string type = b.value("type", "");
        const int x = b.value("x", -1);
        const int y = b.value("y", -1);
        const std::string oStr = b.value("orientation", "NONE");
        const GameObject::Orientation o = OrientationFromString(oStr);

        if (!grid.IsInside(x, y))
            continue;

        // Prevent double placement if something already there
        GridCell* cell = grid.GetCell(x, y);
        if (!cell) continue;
        if (cell->gameObject || cell->conveyorId != INVALID_CONVEYOR) continue;

        // Conveyor
        if (type == "Conveyor")
        {
            ConveyorManager::GetInstance().Create(x, y, o);
            continue;
        }

        // Miner
        if (type == "Miner")
        {
            auto* m = ObjectManager::GetInstance().Spawn<Miner>(o);
            grid.PlaceObject(m, x, y);
            continue;
        }

        // Crafter
        if (type == "Crafter")
        {
            std::string recipeId = b.value("recipeId", "iron_ingot");
            auto* c = ObjectManager::GetInstance().Spawn<Crafter>(o, recipeId);
            grid.PlaceObject(c, x, y);
            continue;
        }
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

            // Conveyor
            if (cell->conveyorId != INVALID_CONVEYOR)
            {
                json b;
                b["type"] = "Conveyor";
                b["x"] = x;
                b["y"] = y;

                b["orientation"] = OrientationToString(ConveyorManager::GetInstance().GetOrientation(cell->conveyorId));

                out["buildings"].push_back(b);
                continue;
            }

            // GameObject
            if (!cell->gameObject) continue;

            GameObject* go = cell->gameObject;

            // Only save buildings you care about
            if (go->type != GameObject::ObjectType::MINER &&
                go->type != GameObject::ObjectType::CRAFTER 
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
        f << out.dump(2); // Pretty print
    }
    catch (const std::exception& e)
    {
        std::cout << "MapLoader::SaveBuildings: write error: " << e.what() << "\n";
        return false;
    }

    std::cout << "MapLoader::SaveBuildings: saved to " << path << "\n";
    return true;
}

bool MapLoader::SaveBuildingsAsync(const std::string& path)
{
    if (sIsSaving.exchange(true)) {
        printf("MapLoader::SaveBuildings: ALREADY SAVING\n");
        return false;
    }

    auto snapshot = CollectBuildingsSnapshot();

    std::thread([path, snapshot = std::move(snapshot)]() mutable
        {
            bool success = false;
            try
            {
                json out = BuildBuildingsJson(snapshot);
                success = WriteJsonToFileAtomic(path, out);
            }
            catch (...) {}

            if (success) std::cout << "MapLoader::SaveBuildings: saved to " << path << "\n";
            else    std::cout << "MapLoader::SaveBuildings: FAILED saving to " << path << "\n";

            sIsSaving.store(false);
        }).detach();

    std::cout << "MapLoader::SaveBuildings: saving started...\n";
    return true;
}

std::vector<BuildingSaveData> MapLoader::CollectBuildingsSnapshot()
{
    GridManager& grid = GridManager::GetInstance();
    std::vector<BuildingSaveData> out;

    const int w = grid.GetWidth();
    const int h = grid.GetHeight();

    out.reserve(w * h / 4);

    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            GridCell* cell = grid.GetCell(x, y);
            if (!cell) continue;

            // Conveyor
            if (cell->conveyorId != INVALID_CONVEYOR)
            {
                BuildingSaveData b;
                b.type = "Conveyor";
                b.x = x; b.y = y;
                b.orientation = ConveyorManager::GetInstance().GetOrientation(cell->conveyorId);
                out.push_back(std::move(b));
                continue;
            }

            // GameObject buildings
            if (!cell->gameObject) continue;

            GameObject* go = cell->gameObject;

            if (go->type == GameObject::ObjectType::MINER)
            {
                BuildingSaveData b;
                b.type = "Miner";
                b.x = x; b.y = y;
                b.orientation = go->orientation;
                out.push_back(std::move(b));
            }
            else if (go->type == GameObject::ObjectType::CRAFTER)
            {
                auto* c = static_cast<Crafter*>(go);

                BuildingSaveData b;
                b.type = "Crafter";
                b.x = x; b.y = y;
                b.orientation = go->orientation;
                b.recipeId = c->GetRecipeId();
                b.hasRecipe = true;
                out.push_back(std::move(b));
            }
        }

    return out;
}

json MapLoader::BuildBuildingsJson(const std::vector<BuildingSaveData>& snapshot)
{
    json out;
    out["buildings"] = json::array();

    for (const auto& b : snapshot)
    {
        json j;
        j["type"] = b.type;
        j["x"] = b.x;
        j["y"] = b.y;
        j["orientation"] = OrientationToString(b.orientation);
        if (b.hasRecipe) j["recipeId"] = b.recipeId;
        out["buildings"].push_back(std::move(j));
    }
    return out;
}

bool MapLoader::WriteJsonToFileAtomic(const std::string& path, const json& j)
{
    const std::string tmp = path + ".tmp";

    std::ofstream f(tmp, std::ios::binary);
    if (!f.is_open())
    {
        std::cout << "MapLoader: cannot open tmp file " << tmp << "\n";
        return false;
    }

    f << j.dump(2);
    f.close();

    std::remove(path.c_str());
    return std::rename(tmp.c_str(), path.c_str()) == 0;
}

bool MapLoader::IsSaving()
{
    return sIsSaving.load();
}

