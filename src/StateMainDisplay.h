#ifndef __STATE_MAIN_DISPLAY_H__
#define __STATE_MAIN_DISPLAY_H__

#include "State.h"
#include "Debug.h"

class StateMainDisplay : public State {
	public:
		void onEnter();
		void update();
};

#endif // __STATE_MAIN_DISPLAY_H__