#ifndef __BUTTON_HANDLER_H__
#define __BUTTON_HANDLER_H__

#include <Arduino.h>

#define NUM_BUTTONS 8

#define BUTTON_FREQ_UP		0
#define BUTTON_FREQ_DOWN	1
#define BUTTON_INFO   		2
#define BUTTON_STORE1 		3
#define BUTTON_STORE2 		4
#define BUTTON_STORE3 		5
#define BUTTON_STORE4 		6
#define BUTTON_MENU			7

#define BUTTON_STATE_UP     HIGH
#define BUTTON_STATE_DOWN   LOW


class ButtonHandler {
	public:
		static void setup();
		static void update();

		static bool isUp(uint8_t button);
		static bool isDown(uint8_t button);

		static bool isReleased(uint8_t button);
		static bool isPressed(uint8_t button);

	private:
		static void _updateRotEnc();

		static uint8_t _buttonState[];
		static uint8_t _lastButtonState[];
		static bool _hasChanged[];
		static unsigned long _lastDebounceTime[];
		static unsigned long _lastChangeTime[];

		static volatile bool _turnDetected;
		static volatile bool _turnedUp;
};

#endif // __BUTTON_HANDLER_H__