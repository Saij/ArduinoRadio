#include "StateMainDisplay.h"
#include "DisplayHandler.h"
#include "ButtonHandler.h"
#include "SettingsHandler.h"
#include "TunerHandler.h"
#include "Debug.h"

#include <Arduino.h>

#define FREQ_DISPLAY_BUF_LEN	12

// This is a special format routine used to format frequencies as strings with leading blanks.
// up to 5 digits only ("    0".."99999")
// *s MUST be able to hold the characters
void _int16ToS(char *s, uint16_t val) {
	uint8_t n = 5;

	while (n > 0) {
		n--;
		if ((n == 4) || (val > 0)) {
	  		s[n] = '0' + (val % 10);
	  		val = val / 10;
		} else {
	  		s[n] = ' ';
		}
	}
}

void StateMainDisplay::onEnter() {
	debugPrintf(F("Entering Main Display State"));
	DisplayHandler::clearDisplay();
}

void StateMainDisplay::update() {
	uint16_t currentFrequency = TunerHandler::getFrequency();

	StateMainDisplay::_displayFrequency(currentFrequency);
}

void StateMainDisplay::_displayFrequency(uint16_t frequency) {
	char buffer[FREQ_DISPLAY_BUF_LEN];

	if ((buffer != NULL)) {
    	*buffer = '\0';

		// " ff.ff MHz" or "fff.ff MHz"
      	_int16ToS(buffer, frequency);

      	// insert decimal point
      	buffer[5] = buffer[4]; 
      	buffer[4] = buffer[3]; 
      	buffer[3] = '.';

	    // append units
      	strcpy(buffer + 6, " MHz");
    }
}