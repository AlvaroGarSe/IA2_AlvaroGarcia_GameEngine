#pragma once
#include "IConveyorSystem.h"
#include "ConveyorTypes.h"
#include <vector>

class SoAConveyorSystem : public IConveyorSystem
{
public:
    ConveyorId Create(float x, float y, GameObject::Orientation o) override;

    void StartAll() override;
    void UpdateAll(uint32_t now) override;
    void RenderAll() override;
    
    bool CanAcceptItem(ConveyorId id) const override;
    bool InsertItem(ConveyorId id, const Item& item) override;
    bool TryExtractItem(ConveyorId id, Item& out) override;
    SDL_Point GetWorldPos(ConveyorId id) const override;
    GameObject::Orientation GetOrientation(ConveyorId id) const override;

    int IndexFromId(ConveyorId id) const { return id - 1; }
    bool ValidIndex(int i) const { return i >= 0 && i < (int)mX.size(); }

private:
    // SoA arrays
    std::vector<float> mX, mY;
    std::vector<GameObject::Orientation> mOri;
    std::vector<uint32_t> mNextMoveTime;
    std::vector<bool> mActive;

    // flattened [conveyorIndex * SLOT_COUNT + slot]
    std::vector<ItemType> mSlotType;

    uint32_t mMoveIntervalMs = 300;

    bool SlotEmpty(int convIndex, int slot) const { return mSlotType[convIndex * SLOT_COUNT + slot] == ItemType::NONE; }
    void SetSlot(int convIndex, int slot, ItemType t) { mSlotType[convIndex * SLOT_COUNT + slot] = t; }
};