#include "ConveyorManager.h"
#include "AoSConveyorSystem.h"
#include "SoAConveyorSystem.h"

void ConveyorManager::Init(ConveyorMode mode)
{
    if (mode == ConveyorMode::AoS) mImpl = std::make_unique<AoSConveyorSystem>();
    else                          mImpl = std::make_unique<SoAConveyorSystem>();
}

ConveyorId ConveyorManager::Create(int cX, int cY, GameObject::Orientation o) { return mImpl->Create(cX, cY, o); }
ConveyorId ConveyorManager::CreateConveyorRuntime(int cX, int cY, GameObject::Orientation o) { return mImpl->CreateConveyorRuntime(cX, cY, o); }

void ConveyorManager::UpdateAll(uint32_t now) { mImpl->UpdateAll(now); }
void ConveyorManager::StartAll() { mImpl->StartAll(); }

void ConveyorManager::RenderAll() { mImpl->RenderAll(); }
void ConveyorManager::RenderAllItemsBehind() { mImpl->RenderAllItemsBehind(); }

void ConveyorManager::RotateConveyor(ConveyorId id, int dir) { mImpl->RotateConveyor(id, dir); }
void ConveyorManager::RemoveConveyor(ConveyorId id) { mImpl->RemoveConveyor(id); }

bool ConveyorManager::CanAcceptItem(ConveyorId id) const { return mImpl->CanAcceptItem(id); }
bool ConveyorManager::InsertItem(ConveyorId id, const Item& item) { return mImpl->InsertItem(id, item); }

SDL_Point ConveyorManager::GetWorldPos(ConveyorId id) const { return mImpl->GetWorldPos(id); }
GameObject::Orientation ConveyorManager::GetOrientation(ConveyorId id) const { return mImpl->GetOrientation(id); }

bool ConveyorManager::IsSoAMode() const { return typeid(*mImpl) == typeid(SoAConveyorSystem); }