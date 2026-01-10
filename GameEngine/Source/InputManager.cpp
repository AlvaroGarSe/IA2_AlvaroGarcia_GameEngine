#include "InputManager.h"

void InputManager::BeginFrame()
{
    // Los estados "edge" duran solo un frame
    memset(mKeyDown, false, sizeof(mKeyDown));
    memset(mKeyUp, false, sizeof(mKeyUp));
}

void InputManager::ProcessEvent(const SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        mKeyHeld[e.key.keysym.scancode] = true;
        mKeyDown[e.key.keysym.scancode] = true;
    }
    else if (e.type == SDL_KEYUP)
    {
        mKeyHeld[e.key.keysym.scancode] = false;
        mKeyUp[e.key.keysym.scancode] = true;
    }
}

bool InputManager::GetKey(int scancode) const
{
    return mKeyHeld[scancode];
}

bool InputManager::GetKeyDown(int scancode) const
{
    return mKeyDown[scancode];
}

bool InputManager::GetKeyUp(int scancode) const
{
    return mKeyUp[scancode];
}
