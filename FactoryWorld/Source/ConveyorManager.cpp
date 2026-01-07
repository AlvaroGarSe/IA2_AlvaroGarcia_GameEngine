#include "ConveyorManager.h"
#include "AoSConveyorSystem.h"
#include "SoAConveyorSystem.h"

void ConveyorManager::Init(ConveyorMode mode)
{
    if (mode == ConveyorMode::AoS) mImpl = std::make_unique<AoSConveyorSystem>();
    else                          mImpl = std::make_unique<SoAConveyorSystem>();
}

ConveyorId ConveyorManager::Create(float x, float y, GameObject::Orientation o) { return mImpl->Create(x, y, o); }
void ConveyorManager::UpdateAll(uint32_t now) { mImpl->UpdateAll(now); }
void ConveyorManager::RenderAll() { mImpl->RenderAll(); }
void ConveyorManager::StartAll() { mImpl->StartAll(); }
bool ConveyorManager::CanAcceptItem(ConveyorId id) const { return mImpl->CanAcceptItem(id); }
bool ConveyorManager::InsertItem(ConveyorId id, const Item& item) { return mImpl->InsertItem(id, item); }
bool ConveyorManager::TryExtractItem(ConveyorId id, Item& out) { return mImpl->TryExtractItem(id, out); }
SDL_Point ConveyorManager::GetWorldPos(ConveyorId id) const { return mImpl->GetWorldPos(id); }
GameObject::Orientation ConveyorManager::GetOrientation(ConveyorId id) const { return mImpl->GetOrientation(id); }
