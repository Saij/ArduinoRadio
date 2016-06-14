#ifndef __BUTTON_HANDLER_H__
#define __BUTTON_HANDLER_H__

#include "Button.h"

#define NUM_BUTTONS 8

#define BUTTON_VOL_UP		0
#define BUTTON_VOL_DOWN		1
#define BUTTON_FREQ_PREV   	2
#define BUTTON_FREQ_NEXT  	3
#define BUTTON_STORE1 		4
#define BUTTON_STORE2 		5
#define BUTTON_STORE3 		6
#define BUTTON_STORE4 		7

class ButtonHandler {
  	public:
    	static void setup();
    	static void update();
    	static Button* getButton(uint8_t btnNum);

  	private:
    	static Button _buttons[];
};

#endif // __BUTTON_HANDLER_H__