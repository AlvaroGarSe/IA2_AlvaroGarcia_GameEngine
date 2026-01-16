#pragma once
#include "ConveyorTypes.h"
#include "GameObject.h"
#include "Item.h"

// This class is used for inheritance for AoS and SoA for the conveyors
class IConveyorSystem
{
public:
    virtual ~IConveyorSystem() = default;

    virtual ConveyorId Create(int cX, int cY, GameObject::Orientation o) = 0;
    virtual ConveyorId CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o) = 0;

    virtual void StartAll() = 0;
    virtual void UpdateAll(uint32_t nowTicks) = 0;
    virtual void RenderAll() = 0;

    virtual void RotateConveyor(ConveyorId id, int dir) = 0;

    virtual bool CanAcceptItem(ConveyorId id) const = 0;
    virtual bool InsertItem(ConveyorId id, const Item& item) = 0;

    virtual void RemoveConveyor(ConveyorId id) = 0;

    virtual SDL_Point GetWorldPos(ConveyorId id) const = 0;
    virtual GameObject::Orientation GetOrientation(ConveyorId id) const = 0;
};