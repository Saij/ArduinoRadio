#ifndef __STATE_H__
#define __STATE_H__

#include "Debug.h"

class State {
	public:
		virtual void onEnter() {
			debugPrintf(F("State - onEnter"));
		};
		virtual void onExit() {
			debugPrintf(F("State - onExit"));
		};
		virtual void update() {
			debugPrintf(F("State - update"));
		};
};

#endif // __STATE_H__