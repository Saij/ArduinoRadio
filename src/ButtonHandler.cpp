#include "ButtonHandler.h"
#include "Debug.h"

#define PIN_BTN_DATAIN    	11
#define PIN_BTN_CLKEN     	12
#define PIN_BTN_CLK       	13
#define PIN_BTN_PLOAD     	A2

#define NUM_74HC165       	1
#define DATA_WIDTH        	NUM_74HC165 * 8
#define PULSE_WIDTH_USEC  	5	

#define DEBOUNCE_DELAY  	50
#define CHANGE_DELAY    	50

#define ROTENC_IRP			0
#define ROTENC_CLK			3
#define ROTENC_DATA			4

#define TURN_NONE			0
#define TURN_UP				1
#define TURN_DOWN			2

uint8_t ButtonHandler::_buttonState[NUM_BUTTONS] = {BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP};
uint8_t ButtonHandler::_lastButtonState[NUM_BUTTONS] = {BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP, BUTTON_STATE_UP};
bool ButtonHandler::_hasChanged[NUM_BUTTONS] = {false, false, false, false, false, false, false, false};
unsigned long ButtonHandler::_lastDebounceTime[NUM_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long ButtonHandler::_lastChangeTime[NUM_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0};

volatile uint8_t ButtonHandler::_detectedTurn = TURN_NONE;
uint8_t ButtonHandler::_handledTurn = TURN_NONE;

void ButtonHandler::setup() {
	debugPrintf(F("Initialize Buttons"));

	pinMode(ROTENC_DATA, INPUT);
	pinMode(ROTENC_CLK, INPUT);
  
	pinMode(PIN_BTN_PLOAD, OUTPUT);
	pinMode(PIN_BTN_CLKEN, OUTPUT);
	pinMode(PIN_BTN_CLK, OUTPUT);
	pinMode(PIN_BTN_DATAIN, INPUT);

	digitalWrite(PIN_BTN_CLK, LOW);
	digitalWrite(PIN_BTN_PLOAD, HIGH);

	attachInterrupt(ROTENC_IRP, ButtonHandler::_updateRotEnc, FALLING);
}

void ButtonHandler::update() {
	// Trigger a parallel load to latch the state of the data lines
	digitalWrite(PIN_BTN_CLKEN, HIGH);
	digitalWrite(PIN_BTN_PLOAD, LOW);
	delayMicroseconds(PULSE_WIDTH_USEC);
	digitalWrite(PIN_BTN_PLOAD, HIGH);
	digitalWrite(PIN_BTN_CLKEN, LOW);

	// Loop to read each bit value from the serial out line of the SN74HC165N.
	for (uint8_t i = 0; i < DATA_WIDTH; i++) {
		if (i < NUM_BUTTONS) {
			uint8_t state = digitalRead(PIN_BTN_DATAIN);
			uint8_t button = NUM_BUTTONS - i - 1;

			if (state != ButtonHandler::_lastButtonState[button]) {
				ButtonHandler::_lastDebounceTime[button] = millis();
			}

			if ((millis() - ButtonHandler::_lastDebounceTime[button]) > DEBOUNCE_DELAY) {
				if (state != ButtonHandler::_buttonState[button]) {
					ButtonHandler::_buttonState[button] = state;
					ButtonHandler::_lastChangeTime[button] = millis();
					ButtonHandler::_hasChanged[button] = true;
				} else {
					ButtonHandler::_hasChanged[button] = false;
				}
			}

			ButtonHandler::_lastButtonState[button] = state;

			#ifdef DEBUG
				if (ButtonHandler::isReleased(button)) {
					debugPrintf(F("Button %d released"), button);
				}

				if (ButtonHandler::isPressed(button)) {
					debugPrintf(F("Button %d pressed"), button);
				}
			#endif
		}
	
		// Pulse the Clock (rising edge shifts the next bit).
		digitalWrite(PIN_BTN_CLK, HIGH);
		delayMicroseconds(PULSE_WIDTH_USEC);
		digitalWrite(PIN_BTN_CLK, LOW);
	}

	// Rotary Encoder
	ButtonHandler::_handledTurn = ButtonHandler::_detectedTurn;
	ButtonHandler::_detectedTurn = TURN_NONE;
}

bool ButtonHandler::isTurnedUp() {
	return ButtonHandler::_handledTurn == TURN_UP;
}

bool ButtonHandler::isTurnedDown() {
	return ButtonHandler::_handledTurn == TURN_DOWN;
}

bool ButtonHandler::isUp(uint8_t button) {
	return ButtonHandler::_buttonState[button] == BUTTON_STATE_UP;
}

bool ButtonHandler::isDown(uint8_t button) {
	return ButtonHandler::_buttonState[button] == BUTTON_STATE_DOWN;
}

bool ButtonHandler::isPressed(uint8_t button) {
	return ButtonHandler::_hasChanged[button] && ButtonHandler::_buttonState[button] == BUTTON_STATE_DOWN;
}

bool ButtonHandler::isReleased(uint8_t button) {
	return ButtonHandler::_hasChanged[button] && ButtonHandler::_buttonState[button] == BUTTON_STATE_UP;
}

void ButtonHandler::_updateRotEnc() {
	if (digitalRead(ROTENC_CLK) == digitalRead(ROTENC_DATA)) {
		ButtonHandler::_detectedTurn = TURN_UP;
	} else {
		ButtonHandler::_detectedTurn = TURN_DOWN;
	}
}