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

	bool mKeyHeld[SDL_NUM_SCANCODES]{};
	bool mKeyDown[SDL_NUM_SCANCODES]{};
	bool mKeyUp[SDL_NUM_SCANCODES]{};	

	/*****************************************************************************/

public:
	// Reset of the states
	void BeginFrame();
	void ProcessEvent(const SDL_Event& e);

	bool GetKey(int scancode) const;      // Held
	bool GetKeyDown(int scancode) const;  // Pressed this frame
	bool GetKeyUp(int scancode) const;    // Released this frame

	SDL_Event* GetSDLEvent() { return &e; };

	/*****************************************************************************/

};