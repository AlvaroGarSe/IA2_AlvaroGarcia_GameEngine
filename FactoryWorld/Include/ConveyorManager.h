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

    ConveyorId Create(float worldX, float worldY, GameObject::Orientation o);

    void UpdateAll(uint32_t nowTicks);
    void RenderAll();
    void StartAll();

    bool CanAcceptItem(ConveyorId id) const;
    bool InsertItem(ConveyorId id, const Item& item);

    bool TryExtractItem(ConveyorId id, Item& out);

    SDL_Point GetWorldPos(ConveyorId id) const;
    GameObject::Orientation GetOrientation(ConveyorId id) const;

private:
    ConveyorManager() {};

    std::unique_ptr<IConveyorSystem> mImpl;    
};