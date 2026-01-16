#include "Crafter.h"
#include "TimeManager.h"
#include "GridManager.h"
#include "Conveyor.h"
#include "Item.h"
#include "RecipeManager.h"
#include "ConveyorManager.h"
#include "AssetManager.h"
#include "GraphicManager.h"

Crafter::Crafter(Orientation orientation, std::string recipeId)
{
    type = ObjectType::CRAFTER;

    SetPosition(0, 0);

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
        break;
    }

    isActive = false;
	isCrafting = false;
    isOn = false;

    RecipeId = recipeId;

    const Recipe* r = RecipeManager::GetInstance().GetRecipeById(recipeId);
    if (!r)
    {
        printf("Crafter: invalid recipe id '%s'. Setting recipe to 'iron_ingot'\n", RecipeId.c_str());
        RecipeId = "iron_ingot";
        r = RecipeManager::GetInstance().GetRecipeById(RecipeId);
    }

    craftingMaxTime = r->craftTimeMs;
    craftingCurrentTime = 0;

    mNeed.fill(0);
    mHave.fill(0);
    mInputSlots.clear();
    mOutputItem = nullptr;

    RebuildRecipeCache();

	texturePath = "Media/Crafter1.png";
}

Crafter::~Crafter()
{
    for (Item* item : mInputSlots) delete item;
	delete mOutputItem;
}

void Crafter::Start()
{
	GameObject::Start();
    isOn = true;
}

void Crafter::StopCrafter()
{
    isOn = false;
	isCrafting = false;
}

bool Crafter::CanAcceptInput() const
{
    for (Item* slot: mInputSlots)
    {
        if (!slot)
        {
            return true;
        }
    }
    return false;
}

bool Crafter::InsertInput(const Item& item)
{
    if (!CanAcceptInputType(item.type))
        return false;

    for (Item*& slot : mInputSlots)
    {
        if (!slot)
        {
            slot = new Item(item);
            mHave[(int)item.type] += 1;
            TryStartCraft();
            return true;
        }
    }
    return false;
}

bool Crafter::HasOutput() const
{
    return mOutputItem != nullptr;
}

void Crafter::TryStartCraft()
{
    if (isCrafting || mOutputItem) return;

    const Recipe* r = RecipeManager::GetInstance().GetRecipeById(RecipeId);
    if (!r) return;

    if (!HasAllInputsForRecipe()) return;

    isCrafting = true;
    craftingMaxTime = r->craftTimeMs;

    Uint32 now = TimeManager::GetInstance().getTicks();
    craftingCurrentTime = now + craftingMaxTime;
}

void Crafter::FinishCraft()
{
    const Recipe* r = RecipeManager::GetInstance().GetRecipeById(RecipeId);
    if (!r) { isCrafting = false; return; }

    // Consume inputs
    for (const auto& in : r->inputs)
        ConsumeType(in.type, in.amount);

    // Produce
    if (!r->outputs.empty())
        mOutputItem = new Item{ r->outputs[0].type };

    isCrafting = false;
}

void Crafter::RebuildRecipeCache()
{
    // Delete all the current items and reset params
    for (Item* it : mInputSlots)
        delete it;
    mInputSlots.clear();
    if (mOutputItem)
    {
        delete mOutputItem;
        mOutputItem = nullptr;
    }
    isCrafting = false;
    craftingCurrentTime = 0;

    mNeed.fill(0);
    mHave.fill(0);

    const Recipe* r = RecipeManager::GetInstance().GetRecipeById(RecipeId);
    if (!r) return;

    itemTexture = AssetManager::GetInstance().GetTexture(r->itemSpritePath);

    int totalNeed = 0;
    for (const ItemStack& in : r->inputs)
    {
        mNeed[(int)in.type] += in.amount;
        totalNeed += in.amount;
    }

    // Resize the slots to the needed amount
    mInputSlots.assign(totalNeed, nullptr);
}

bool Crafter::CanAcceptInputType(ItemType t) const
{
    int idx = (int)t;
    if (idx < 0 || idx >= ITEMTYPE_COUNT) return false;

    // not in recipe?
    if (mNeed[idx] <= 0) return false;

    // already have enough?
    if (mHave[idx] >= mNeed[idx]) return false;

    // need a free slot
    for (Item* s : mInputSlots)
        if (!s) return true;

    return false;
}

bool Crafter::HasAllInputsForRecipe() const
{
    for (int i = 0; i < ITEMTYPE_COUNT; ++i)
        if (mNeed[i] > 0 && mHave[i] < mNeed[i])
            return false;
    return true;
}

void Crafter::ConsumeType(ItemType t, int count)
{
    int idx = (int)t;
    int toRemove = count;

    for (int i = 0; i < (int)mInputSlots.size() && toRemove > 0; ++i)
    {
        Item* it = mInputSlots[i];
        if (it && it->type == t)
        {
            delete it;
            mInputSlots[i] = nullptr;
            mHave[idx] -= 1;
            toRemove--;
        }
    }
}

void Crafter::Update(uint32_t now)
{
    if (!isActive || !isOn) return;

    if (isCrafting && now >= craftingCurrentTime)
    {
		FinishCraft();
    }

    if (HasOutput())
    {
        GridManager& gridManager = GridManager::GetInstance();

        SDL_Point posGrid = gridManager.WorldToGrid((int)transform.x, (int)transform.y);

        GridCell* nextCell = gridManager.GetAdjacentCell(posGrid.x, posGrid.y, orientation);

        // Checks if the cell is valid and has an item object in it
        if (!nextCell) return;

        ConveyorId convId = nextCell->conveyorId;
        if (convId == INVALID_CONVEYOR) return;

        if (ConveyorManager::GetInstance().InsertItem(convId, *mOutputItem))
        {
            delete mOutputItem;
            mOutputItem = nullptr;
        }
    }
}

void Crafter::Render()
{
    // Render the crafter
    GameObject::Render();

    // These offsets just make the squares appear on top of the belt
    const int cellSize = GridManager::GetInstance().GetCellSize();

    // World-space item size (in "world pixels")
    const float itemSizeWorld = 32;

    // Conveyor top-left in world space
    float baseWorldX = transform.x;
    float baseWorldY = transform.y;

    // Center item inside the cell (world space)
    const float centerOffsetWorld = (cellSize - itemSizeWorld / 2) / 2;

    // Start position (world space)
    int startX = baseWorldX + centerOffsetWorld;
    int startY = baseWorldY + centerOffsetWorld;

    float sx = itemSizeWorld / itemTexture->getWidth();
    float sy = itemSizeWorld / itemTexture->getHeight();

    Transform itTransf;
    itTransf.x = startX - centerOffsetWorld * 0.5f;
    itTransf.y = startY - centerOffsetWorld * 0.5f;
    itTransf.rotation = 0.0f;
    itTransf.scaleX = sx;
    itTransf.scaleY = sy;

    GraphicManager::GetInstance().DrawTexture(itemTexture, itTransf);

}

void Crafter::ChangeRecipe(const std::string& newId)
{
    const Recipe* newRecipe = RecipeManager::GetInstance().GetRecipeById(newId);
    if (!newRecipe) return;

    RecipeId = newId;
    craftingMaxTime = newRecipe->craftTimeMs;
    craftingCurrentTime = 0;
    isCrafting = false;

    RebuildRecipeCache();
}

void Crafter::ChangeNextRecipe()
{
    const Recipe* r = RecipeManager::GetInstance().GetNextRecipeById(RecipeId);

    RecipeId = r->id;
    craftingMaxTime = r->craftTimeMs;
    craftingCurrentTime = 0;
    isCrafting = false;

    RebuildRecipeCache();
}
