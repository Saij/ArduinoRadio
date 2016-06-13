#ifndef __BUTTON_HANDLER_H__
#define __BUTTON_HANDLER_H__

#include "Button.h"

#define NUM_BUTTONS 8

#define BUTTON_MENU   0
#define BUTTON_EXIT   1
#define BUTTON_STORE1 2
#define BUTTON_STORE2 3
#define BUTTON_STORE3 4
#define BUTTON_STORE4 5
#define BUTTON_STORE5 6
#define BUTTON_STORE6 7

class ButtonHandler {
  	public:
    	static void setup();
    	static void update();
    	static Button* getButton(uint8_t btnNum);

  	private:
    	static Button _buttons[];
};

#endif // __BUTTON_HANDLER_H__