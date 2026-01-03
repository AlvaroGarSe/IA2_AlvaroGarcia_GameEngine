#include "Conveyor.h"
#include  "GridManager.h"
#include "TimeManager.h"
#include "GraphicManager.h"
#include "Crafter.h"

Conveyor::Conveyor(Orientation orientation)
{
    SetPosition(0, 0);

    isActive = true;

    transform.scaleX = 0.5;
    transform.scaleY = 0.5;

    this->orientation = orientation;

    switch (orientation)
    {
    case GameObject::Orientation::NONE:
		transform.rotation = 0;
        break;
    case GameObject::Orientation::NORTH:
        transform.rotation = 0;
        break;
    case GameObject::Orientation::SOUTH:
        transform.rotation = 180;
        break;
    case GameObject::Orientation::EAST:
        transform.rotation = 90;
        break;
    case GameObject::Orientation::WEST:
        transform.rotation = 270;
        break;
    default:
        transform.rotation = 0;
        break;
    }

	type = ObjectType::CONVEYOR;

    texturePath = "Media/Conveyor.png";

    // slots start empty
    for (auto& s : mSlots) s = nullptr;

    // start stepping immediately
    mNextMoveTime = 0;
}

Conveyor::~Conveyor()
{
    for (Item* item : mSlots) delete item;
}

void Conveyor::Update()
{
    if (!isActive) return;

	bool hasItem = false;
    for (auto* it : mSlots)
        if (it) { hasItem = true; break; }

    if (!hasItem) return;

    Uint32 currentTime = TimeManager::GetInstance().getTicks();

    if (currentTime >= mNextMoveTime)
    {
        mNextMoveTime = currentTime + mMoveIntervalMs;

        // First try to pass the item at the end to the next cell
        PassItemNextCell();

        // Then move items forward inside this conveyor
        MoveItemThroughSlots();
    }
}

void Conveyor::Render()
{
    // Render the belt
	GameObject::Render();

    SDL_Renderer* r = GraphicManager::GetInstance().gRenderer;
    if (!r) return;

    // These offsets just make the squares appear on top of the belt
	const int cellSize = GridManager::GetInstance().GetCellSize();
    float zoom = GraphicManager::GetInstance().camera.GetZoom();

    
    int dirX = 0, dirY = 0;
    switch (orientation)
    {
        case Orientation::NORTH: dirX = 0; dirY = -1; break;
        case Orientation::SOUTH: dirX = 0; dirY = 1; break;
        case Orientation::EAST:  dirX = 1; dirY = 0; break;
        case Orientation::WEST:  dirX = -1; dirY = 0; break;
        default: break;
    }

    // World-space item size (in "world pixels")
	const int itemSizeWorld = 12;
    const int slotSpacingWorld = cellSize / SLOT_COUNT; 

    // Conveyor top-left in world space
    float baseWorldX = transform.x;
    float baseWorldY = transform.y;

    // Center item inside the cell (world space)
    const float centerOffsetWorld = (cellSize - itemSizeWorld) / 2;

    // Start position (world space)
    int startX = baseWorldX + centerOffsetWorld;
    int startY = baseWorldY + centerOffsetWorld;

    for (int i = 0; i < SLOT_COUNT; ++i)
    {
        if (!mSlots[i]) continue;

        float offset = (i * slotSpacingWorld) - (cellSize / 2 - slotSpacingWorld / 2);

        int x = startX + dirX * offset;
        int y = startY + dirY * offset;

        SDL_Point screen = GraphicManager::GetInstance().camera.WorldToScreen(x, y);

        int itemSizeScreen = (int)(itemSizeWorld * zoom);

        SDL_Rect rect{ screen.x, screen.y, itemSizeScreen, itemSizeScreen };
        switch (mSlots[i]->type)
        {
		case ItemType::IronOre:
			SDL_SetRenderDrawColor(r, 121, 193, 247, 255);
			break;
		case ItemType::IronIngot:
			SDL_SetRenderDrawColor(r, 212, 74, 125, 255);
			break;
        default:
            SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            break;
        }
        SDL_RenderFillRect(r, &rect);
    }
}

bool Conveyor::CanAcceptItem() const
{
    return mSlots[0] == nullptr;
}

bool Conveyor::InsertItem(const Item& item)
{
    if (!CanAcceptItem()) return false;
    
	mSlots[0] = new Item(item);

    // Prevent instant teleport: don't allow this belt to step until next interval
    Uint32 currentTime = TimeManager::GetInstance().getTicks();
    if (mNextMoveTime <= currentTime)
        mNextMoveTime = currentTime + mMoveIntervalMs;

	return true;
}

bool Conveyor::TryExtractItem(Item& outItem)
{
    int last = SLOT_COUNT - 1;
    if (!mSlots[last]) return false;

    outItem = *mSlots[last];
    delete mSlots[last];
    mSlots[last] = nullptr;
    return true;
}

void Conveyor::MoveItemThroughSlots()
{
    for (int i = SLOT_COUNT - 1; i > 0; --i)
    {
        if (mSlots[i] == nullptr && mSlots[i - 1] != nullptr)
        {
            mSlots[i] = mSlots[i - 1];
            mSlots[i - 1] = nullptr;
        }
    }

}

void Conveyor::PassItemNextCell()
{
    // If last slot empty, nothing to pass
    int last = SLOT_COUNT - 1;
    if (!mSlots[last]) return;

    GridManager& gridManager = GridManager::GetInstance();

    SDL_Point posGrid = gridManager.WorldToGrid((int)transform.x, (int)transform.y);

    GridCell* nextCell = gridManager.GetAdjacentCell(posGrid.x, posGrid.y, orientation);

	// Checks if the cell is valid and has an item object in it
    if (!nextCell || !nextCell->gameObject) return;

    if (nextCell->gameObject->type == ObjectType::CONVEYOR)
    {
		Conveyor* nextConv = static_cast<Conveyor*>(nextCell->gameObject);

        if (nextConv->CanAcceptItem())
        {
            Item item;
			if (TryExtractItem(item))
			{
				nextConv->InsertItem(item);
			}
        }		
    }
    else if (nextCell->gameObject->type == ObjectType::CRAFTER)
    {
        Crafter* nextCrafter = static_cast<Crafter*>(nextCell->gameObject);

        if (nextCrafter->CanAcceptInput())
        {
            Item item;
            if (TryExtractItem(item))
            {
                nextCrafter->InsertInput(item);
            }
        }
    }
}
