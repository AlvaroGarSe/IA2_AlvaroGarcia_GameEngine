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

	// Keyboard
	bool mKeyHeld[SDL_NUM_SCANCODES]{};
	bool mKeyDown[SDL_NUM_SCANCODES]{};
	bool mKeyUp[SDL_NUM_SCANCODES]{};

	// Mouse buttons: SDL buttons go from 1 to 5
	static constexpr int MAX_MOUSE_BUTTONS = 6;
	bool mMouseHeld[MAX_MOUSE_BUTTONS]{};
	bool mMouseDown[MAX_MOUSE_BUTTONS]{};
	bool mMouseUp[MAX_MOUSE_BUTTONS]{};

public:
	// Reset of the states
	void BeginFrame();
	void ProcessEvent(const SDL_Event& e);

	// Keyboard Inputs
	bool GetKey(int scancode) const;      // Held
	bool GetKeyDown(int scancode) const;  // Pressed this frame
	bool GetKeyUp(int scancode) const;    // Released this frame

	// Mouse Inputs
	bool GetMouseButton(int button) const;
	bool GetMouseButtonDown(int button) const;
	bool GetMouseButtonUp(int button) const;

	SDL_Event* GetSDLEvent() { return &e; };
};