#pragma once
#include "Singleton.h"
#include "ConveyorTypes.h"
#include "IConveyorSystem.h"
#include <SDL.h>
#include <memory>

class ConveyorManager : public Singleton<ConveyorManager>
{
    friend class Singleton<ConveyorManager>;

public:
    void Init(ConveyorMode mode);

    ConveyorId Create(int cX, int cY, GameObject::Orientation o);
    ConveyorId CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o);

    void UpdateAll(uint32_t nowTicks);
    void StartAll();
    void RenderAll();

    void RotateConveyor(ConveyorId id, int dir);

    bool CanAcceptItem(ConveyorId id) const;
    bool InsertItem(ConveyorId id, const Item& item);

    void RemoveConveyor(ConveyorId id);

    bool IsSoAMode() const;

    SDL_Point GetWorldPos(ConveyorId id) const;
    GameObject::Orientation GetOrientation(ConveyorId id) const;

private:
    ConveyorManager() {};

    // Stores both systems AoS and SoA for conveyors, to then select the one that is being used
    std::unique_ptr<IConveyorSystem> mImpl;    
};