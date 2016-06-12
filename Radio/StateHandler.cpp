#include "StateHandler.h"
#include "Debug.h"

uint8_t StateHandler::_currentState = STATE_MAIN_DISPLAY;
uint8_t StateHandler::_oldState = STATE_MAIN_DISPLAY;

State* StateHandler::_states[NUM_STATES] = {
	new StateMainDisplay()
};

void StateHandler::setupStates() {
	debugPrintf(F("Initialize States"));
	
	StateHandler::getCurrentState()->onEnter();
}

void StateHandler::updateStates() { 
	if (StateHandler::_currentState != StateHandler::_oldState) {
		// Old State => Exit
		StateHandler::getState(StateHandler::_oldState)->onExit();
		
		// New State => Enter
		StateHandler::getCurrentState()->onEnter();
	}

	StateHandler::getCurrentState()->update();
}

void StateHandler::changeState(uint8_t newState) {
	StateHandler::_oldState = StateHandler::_currentState;
	StateHandler::_currentState = newState;
}

State* StateHandler::getState(uint8_t stateNum) {
	return StateHandler::_states[constrain(stateNum, 0, NUM_STATES - 1)];
}

State* StateHandler::getCurrentState() {
	return StateHandler::getState(StateHandler::_currentState);
}