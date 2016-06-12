#include "ButtonHandler.h"
#include "Debug.h"

#define PIN_BTN_DATAIN    11
#define PIN_BTN_CLKEN     12
#define PIN_BTN_CLK       13
#define PIN_BTN_PLOAD     A2

#define NUM_74HC165       1
#define DATA_WIDTH        NUM_74HC165 * 8
#define PULSE_WIDTH_USEC  5

Button ButtonHandler::_buttons[NUM_BUTTONS];

void ButtonHandler::setupButtons() {
	debugPrintf(F("Initialize Buttons"));
  
	pinMode(PIN_BTN_PLOAD, OUTPUT);
	pinMode(PIN_BTN_CLKEN, OUTPUT);
	pinMode(PIN_BTN_CLK, OUTPUT);
	pinMode(PIN_BTN_DATAIN, INPUT);

	digitalWrite(PIN_BTN_CLK, LOW);
	digitalWrite(PIN_BTN_PLOAD, HIGH);
}

void ButtonHandler::updateButtons() {
	// Trigger a parallel load to latch the state of the data lines
	digitalWrite(PIN_BTN_CLKEN, HIGH);
	digitalWrite(PIN_BTN_PLOAD, LOW);
	delayMicroseconds(PULSE_WIDTH_USEC);
	digitalWrite(PIN_BTN_PLOAD, HIGH);
	digitalWrite(PIN_BTN_CLKEN, LOW);

	// Loop to read each bit value from the serial out line of the SN74HC165N.
	for (int i = 0; i < DATA_WIDTH; i++) {
		if (i < NUM_BUTTONS) {
			ButtonHandler::getButton(NUM_BUTTONS - i - 1)->setState(digitalRead(PIN_BTN_DATAIN));
		}
	
		// Pulse the Clock (rising edge shifts the next bit).
		digitalWrite(PIN_BTN_CLK, HIGH);
		delayMicroseconds(PULSE_WIDTH_USEC);
		digitalWrite(PIN_BTN_CLK, LOW);
	}
}

Button* ButtonHandler::getButton(uint8_t btnNum) {
	return &ButtonHandler::_buttons[constrain(btnNum, 0, NUM_BUTTONS - 1)];
}