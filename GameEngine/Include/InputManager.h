#pragma once
#include "Singleton.h"
#include "SDL.h"

class InputManager :public Singleton<InputManager>
{
  /*****************************************************************************/
  friend class Singleton<InputManager>;
  /*****************************************************************************/

private:
  
  // Keyboard state
  const Uint8*  mCurrentKeyStates;

  // Private constructor to avoid more than one instance
  InputManager() { mCurrentKeyStates = new Uint8;};

  //Event handler
  SDL_Event e;

  /*****************************************************************************/

public:
  
  void Update( void );

  bool GetKey( int scanCode ) const;

  SDL_Event* GetSDLEvent() { return &e; };

  /*****************************************************************************/

};