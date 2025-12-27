#include "InputManager.h"

/*****************************************************************************/

void InputManager::Update(void)
{
  mCurrentKeyStates = SDL_GetKeyboardState(NULL);
}
/*****************************************************************************/

bool InputManager::GetKey(int scanCode) const
{
  return mCurrentKeyStates[scanCode] != 0;
}

/*****************************************************************************/	