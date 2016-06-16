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
		void _displayFrequency();
		void _displayVolume();

		uint16_t _curFrequency = 0;
		uint16_t _curVolume = 0;
};

#endif // __STATE_MAIN_DISPLAY_H__