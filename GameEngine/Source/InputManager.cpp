#include "InputManager.h"

void InputManager::BeginFrame()
{
    // Keyboard edge states
    memset(mKeyDown, false, sizeof(mKeyDown));
    memset(mKeyUp, false, sizeof(mKeyUp));

    // Mouse edge states
    memset(mMouseDown, false, sizeof(mMouseDown));
    memset(mMouseUp, false, sizeof(mMouseUp));
}

void InputManager::ProcessEvent(const SDL_Event& e)
{
    // Keyboard buttons
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

    // Mouse buttons
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int button = e.button.button;
        if (button < MAX_MOUSE_BUTTONS)
        {
            mMouseHeld[button] = true;
            mMouseDown[button] = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
        int button = e.button.button;
        if (button < MAX_MOUSE_BUTTONS)
        {
            mMouseHeld[button] = false;
            mMouseUp[button] = true;
        }
    }
}

// ---------------- Keyboard ----------------

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

// ---------------- Mouse ----------------

bool InputManager::GetMouseButton(int button) const
{
    return button < MAX_MOUSE_BUTTONS && mMouseHeld[button];
}

bool InputManager::GetMouseButtonDown(int button) const
{
    return button < MAX_MOUSE_BUTTONS && mMouseDown[button];
}

bool InputManager::GetMouseButtonUp(int button) const
{
    return button < MAX_MOUSE_BUTTONS && mMouseUp[button];
}
