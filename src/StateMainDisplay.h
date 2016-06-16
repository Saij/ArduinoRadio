#ifndef __STATE_MAIN_DISPLAY_H__
#define __STATE_MAIN_DISPLAY_H__

#include "State.h"
#include "Debug.h"

#include <Arduino.h>

class StateMainDisplay : public State {
	public:
		void onEnter();
		void update();

	private:
		void _displayFrequency(uint16_t frequency);
};

#endif // __STATE_MAIN_DISPLAY_H__