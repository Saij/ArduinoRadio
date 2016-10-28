#include "StateMainDisplay.h"
#include "DisplayHandler.h"
#include "ButtonHandler.h"
#include "SettingsHandler.h"
#include "TunerHandler.h"
#include "Debug.h"

#include <Arduino.h>

#define FREQ_DISPLAY_BUF_LEN	12

#define MIN_VOLUME  0
#define MAX_VOLUME 31

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

	this->_curVolume = SettingsHandler::getVolume();
	this->_curFrequency = TunerHandler::getFrequency();

	this->_displayFrequency();
	this->_displayVolume();
}

void StateMainDisplay::update() {
	/*uint16_t currentFrequency = TunerHandler::getFrequency();

	// Change frequency
	if (ButtonHandler::isRelesed(BUTTON_FREQ_DOWN) || ButtonHandler::isRelesed(BUTTON_FREQ_UP)) {
		TunerHandler::setFrequency(this->_);
	}

	if (ButtonHandler::isPressed(BUTTON_FREQ_UP) && currentFrequency)

	if (currentFrequency != this->_curFrequency) {
		// Frequency changed
		this->_curFrequency = currentFrequency;	
		SettingsHandler::setLastFrequency(currentFrequency);
		this->_displayFrequency();
	}*/

	if (ButtonHandler::isTurnedUp() && this->_curVolume < MAX_VOLUME) {
		this->_curVolume += 1;
		SettingsHandler::setVolume(this->_curVolume);
		TunerHandler::setVolume(this->_curVolume);
		this->_displayVolume();
	}

	if (ButtonHandler::isTurnedDown() && this->_curVolume > MIN_VOLUME) {
		this->_curVolume -= 1;
		SettingsHandler::setVolume(this->_curVolume);
		TunerHandler::setVolume(this->_curVolume);
		this->_displayVolume();
	}
}

void StateMainDisplay::_displayFrequency() {
	char buffer[FREQ_DISPLAY_BUF_LEN];

	// " ff.ff MHz" or "fff.ff MHz"
  	_int16ToS(buffer, this->_curFrequency);

  	// insert decimal point
  	buffer[5] = buffer[4]; 
  	buffer[4] = buffer[3]; 
  	buffer[3] = '.';

    // append units
  	strcpy(buffer + 6, " MHz");

  	DisplayHandler::setCursor(0, 3);
  	DisplayHandler::print(buffer);
}

void StateMainDisplay::_displayVolume() {
	DisplayHandler::setCursor(4, 0);

	for (uint16_t volLevel = 0; volLevel <= 31; volLevel += 2) {
		if (volLevel <= this->_curVolume) {
			if (volLevel + 1 > this->_curVolume) {
				// Half volume bar
				DisplayHandler::write(CHAR_HALF_BLOCK);
			} else {
				// Full volume bar
				DisplayHandler::write(CHAR_FULL_BLOCK);			
			}
		} else {
			// Print space to clear old out
			DisplayHandler::write(' ');
		}
	}
}