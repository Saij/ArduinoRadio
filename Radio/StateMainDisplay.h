#ifndef __STATE_MAIN_DISPLAY_H__
#define __STATE_MAIN_DISPLAY_H__

#include "Debug.h"
#include "State.h"

class StateMainDisplay : public State {
  public:
    void onEnter() {
      debugPrintf(F("State Main Display - onEnter"));
    };
    void onExit() {
      debugPrintf(F("State Main Display - onExit"));
    };
    void update() {
      debugPrintf(F("State Main Display - update"));
    };
};

#endif // __STATE_MAIN_DISPLAY_H__
