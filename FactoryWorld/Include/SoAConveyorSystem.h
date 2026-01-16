#pragma once
#include "IConveyorSystem.h"
#include "ConveyorTypes.h"
#include <vector>

class SoAConveyorSystem : public IConveyorSystem
{
public:
    ConveyorId Create(int cX, int cY, GameObject::Orientation o) override;

    void StartAll() override;
    void UpdateAll(uint32_t now) override;
    void RenderAll() override;

    ConveyorId CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o);
    
    bool CanAcceptItem(ConveyorId id) const override;
    bool InsertItem(ConveyorId id, const Item& item) override;

    void RotateConveyor(ConveyorId id, int dir) override;

    void RemoveConveyor(ConveyorId id) override;

    SDL_Point GetWorldPos(ConveyorId id) const override;
    GameObject::Orientation GetOrientation(ConveyorId id) const override;

    int IndexFromId(ConveyorId id) const { return id - 1; }
    bool ValidIndex(int i) const { return i >= 0 && i < (int)mX.size(); }
    SoAConveyorSystem();

private:
    // Pointer to the texture of all the conveyors becuase they use the same (AssetManager)
    LTexture* convTexture;

    // SoA arrays
    std::vector<int> mX, mY;
    std::vector<GameObject::Orientation> mOri;
    std::vector<uint32_t> mNextMoveTime;
    std::vector<int> mActive;

    // All the slots of all the conveyors: [conveyorIndex * SLOT_COUNT + slot]
    std::vector<ItemType> mSlotType;

    uint32_t mMoveIntervalMs = 300;

    void MoveItemThroughSlots(int i);
    void PassItemNextCell(int i);

    int SlotToIndex(int convIndex, int slot) const { return convIndex * SLOT_COUNT + slot; }
    bool SlotEmpty(int convIndex, int slot) const { return mSlotType[convIndex * SLOT_COUNT + slot] == ItemType::NONE; }
    void SetSlot(int convIndex, int slot, ItemType t) { mSlotType[convIndex * SLOT_COUNT + slot] = t; }

};