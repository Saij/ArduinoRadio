#ifndef __STATES_H__
#define __STATES_H__

#include <Arduino.h>

#include "State.h"
#include "StateMainDisplay.h"

#define NUM_STATES          1

#define STATE_MAIN_DISPLAY  0

class StateHandler {
	public:
		static void setupStates();
		static void updateStates();
		static void changeState(uint8_t newState);
		static State* getState(uint8_t stateNum);
		static State* getCurrentState();
	
	private:
		StateHandler() {};

		static State* _states[];
		static uint8_t _currentState;
		static uint8_t _oldState;
};

#endif // __STATES_H__