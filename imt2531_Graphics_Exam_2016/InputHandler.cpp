#include "InputHandler.h"
#include "globals.h"

void InputHandler::init() {
	//Add new events here and in GameEvent.h

}

void InputHandler::readInput(std::queue<GameEvent>& events) {
	//Handle new events on queue
	while (SDL_PollEvent(&event) != 0) {
		//User requests quit
		if (event.type == SDL_QUIT) {
			gRunning = false;
		} else if (	event.type == SDL_KEYDOWN ||
					event.type == SDL_KEYUP)
		{
			handleKeys(events);
		} else if (	event.type == SDL_MOUSEBUTTONDOWN ||
					event.type == SDL_MOUSEBUTTONUP ||
					event.type == SDL_MOUSEMOTION ||
					event.type == SDL_MOUSEWHEEL)
		{
			handleMouse(events);
		}
	}
	handleHeldKeys(events);
}

void InputHandler::handleKeys(std::queue<GameEvent>& events) {
	ActionEnum action = ActionEnum::NOACTION;
	switch (event.type) {
		case SDL_KEYDOWN:
			keyboardState[event.key.keysym.sym] = true;
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					gRunning = false;
					break;
				case SDLK_n:
					action = ActionEnum::NIGHT;
					break;
				case SDLK_e:
					action = ActionEnum::PRINTPOS;
					break;
				case SDLK_f:
					action = ActionEnum::TOGGLECAMERA;
					break;
				case SDLK_l:
					action = ActionEnum::TOGGLEMOUSE;
					break;
				default:
					break;
			}
			break;

		case SDL_KEYUP:
			keyboardState[event.key.keysym.sym] = false;
			switch (event.key.keysym.sym) {
				default:
					break;
			}	
			break;

		default: 			
			break;
	}
	if (action != ActionEnum::NOACTION)
		events.push({ 0, action });
}

//My version of eventRepeat, this checks if keys are held
void InputHandler::handleHeldKeys(std::queue<GameEvent>& events){
	if (keyboardState[SDLK_a]) events.push({ 0, ActionEnum::LEFT });
	if (keyboardState[SDLK_d]) events.push({ 0, ActionEnum::RIGHT });
	if (keyboardState[SDLK_w]) events.push({ 0, ActionEnum::FORWARD });
	if (keyboardState[SDLK_s]) events.push({ 0, ActionEnum::BACK });
	if (keyboardState[SDLK_SPACE]) events.push({ 0, ActionEnum::UP });
	if (keyboardState[SDLK_c]) events.push({ 0, ActionEnum::DOWN });
}

void InputHandler::handleMouse(std::queue<GameEvent>& events) {
    ActionEnum action = ActionEnum::NOACTION;
    
    switch (event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_MOUSEWHEEL:
            break;
        case SDL_MOUSEMOTION:
			mousePosition.x = event.motion.x;
			mousePosition.y = event.motion.y;
			mouseDelta.x = event.motion.xrel;
			mouseDelta.y = event.motion.yrel;
			action = ActionEnum::MOUSEMOTION;
            break;
    }
	if (action != ActionEnum::NOACTION)
		events.push({ 0, action });
}