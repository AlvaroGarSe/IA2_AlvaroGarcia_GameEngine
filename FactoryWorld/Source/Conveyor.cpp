#include "Conveyor.h"
#include  "GridManager.h"
#include "AssetManager.h"
#include "TimeManager.h"
#include "GraphicManager.h"
#include "ConveyorManager.h"
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

    // Slots start empty
    for (auto& s : mSlots) s = nullptr;

    // Start stepping immediately
    mNextMoveTime = 0;
}

Conveyor::~Conveyor()
{
    // Free the inside items
    for (Item* item : mSlots) delete item;
}

// Used for rendering the items on the conveyor
static const char* ItemSpritePath(ItemType t)
{
    switch (t)
    {
        case ItemType::IronOre:   return "Media/Items/IronOre.png";
        case ItemType::IronIngot: return "Media/Items/IronIngot.png";
        case ItemType::CopperOre:   return "Media/Items/CopperOre.png";
        case ItemType::CopperIngot: return "Media/Items/CopperIngot.png";
        case ItemType::IronRod: return "Media/Items/IronRod.png";
        case ItemType::IronWrench: return "Media/Items/IronWrench.png";
        case ItemType::IronGear: return "Media/Items/IronGear.png";
        case ItemType::CopperGear: return "Media/Items/CopperGear.png";
        default:                  return "Media/Items/UnknownItem.png";
    }
}

void Conveyor::Update(uint32_t now)
{
    if (!isActive) return;

	bool hasItem = false;
    for (auto* it : mSlots)
        if (it) { hasItem = true; break; }

    if (!hasItem) return;

    if (now >= mNextMoveTime)
    {
        mNextMoveTime = now + mMoveIntervalMs;

        // First try to pass the item at the end to the next cell
        PassItemNextCell();

        // Then move items forward inside this conveyor
        MoveItemThroughSlots();
    }
}

void Conveyor::RenderItems()
{
    // These offsets just make the squares appear on top of the belt
	const int cellSize = GridManager::GetInstance().GetCellSize();

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
	const float itemSizeWorld = 32;
    const int slotSpacingWorld = cellSize / SLOT_COUNT; 

    // Conveyor top-left in world space
    float baseWorldX = transform.x;
    float baseWorldY = transform.y;

    // Center item inside the cell (world space)
    const float centerOffsetWorld = (cellSize - itemSizeWorld / 2) / 2;

    // Start position (world space)
    int startX = (int)(baseWorldX + centerOffsetWorld);
    int startY = (int)(baseWorldY + centerOffsetWorld);

    for (int i = 0; i < SLOT_COUNT; ++i)
    {
        if (!mSlots[i]) continue;

        LTexture* itemText = AssetManager::GetInstance().GetTexture(ItemSpritePath(mSlots[i]->type));
        if (!itemText) continue;

        // World-space size for the item
        float sx = itemSizeWorld / itemText->getWidth();
        float sy = itemSizeWorld / itemText->getHeight();

        float offset = (float)((i * slotSpacingWorld) - (cellSize / 2 - slotSpacingWorld / 2));

        Transform itTransf;
        itTransf.x = startX + dirX * offset - itemSizeWorld * 0.25;
        itTransf.y = startY + dirY * offset - itemSizeWorld * 0.25;
        itTransf.rotation = 0.0f;
        itTransf.scaleX = sx;
        itTransf.scaleY = sy;

        GraphicManager::GetInstance().DrawTexture(itemText, itTransf);
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

    if (mNextMoveTime <= currentTime) mNextMoveTime = currentTime + mMoveIntervalMs;

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
    if (!nextCell) return;

    Item item = *mSlots[last];

    // Case A: next cell has a SoA conveyor id
    if (nextCell->conveyorId != INVALID_CONVEYOR)
    {
        if (ConveyorManager::GetInstance().InsertItem(nextCell->conveyorId, item))
        {
            delete mSlots[last];
            mSlots[last] = nullptr;
        }
        return;
    }

    // Case B: Next cell has a normal GameObject (Crafter)
    if (nextCell->gameObject)
    {
        if (nextCell->gameObject->type == ObjectType::CRAFTER)
        {
            Crafter* nextCrafter = static_cast<Crafter*>(nextCell->gameObject);

            if (nextCrafter->CanAcceptInput() && nextCrafter->InsertInput(item))
            {
                delete mSlots[last];
                mSlots[last] = nullptr;
            }
        }
    }
}
