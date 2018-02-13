#pragma	once

#if defined(__linux__)						// If we are using linux.
#include <SDL2/SDL.h>

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDL.h>
#endif

#include <map>
#include <queue>
#include <glm/glm.hpp>
#include <chrono>
#include "GameEvent.h"

//I prefer keyboard state to repeating events
class InputHandler {
public:
	// This ensures that there is only ever one instance of the InputHandler.
	static InputHandler& getInstance() {
		static InputHandler instance;
		return instance;
	}
	
	void init();
	void readInput(std::queue<GameEvent>& events); //Reads input events from SDL
	glm::vec2 getMouseDelta() { return mouseDelta; }
	// This ensures that there is only ever one instance of the InputHandler.
	InputHandler(InputHandler const& copy) = delete;
	void operator=(InputHandler const& copy) = delete;
private:
	SDL_Event event;
	glm::vec2 mousePosition;
	glm::vec2 mouseDelta;
	std::map<int, bool> keyboardState;	//1 = key is held down, 0 = key is not held down

	InputHandler() {};

	void handleHeldKeys(std::queue<GameEvent>& events);
	void handleKeys(std::queue<GameEvent>& events);
	void handleMouse(std::queue<GameEvent>& events);
};