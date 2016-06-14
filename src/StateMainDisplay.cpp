#include "StateMainDisplay.h"
#include "DisplayHandler.h"
#include "ButtonHandler.h"
#include "SettingsHandler.h"
#include "Debug.h"

void StateMainDisplay::onEnter() {
	debugPrintf(F("Entering Main Display State"));
	DisplayHandler::clearDisplay();
}

void StateMainDisplay::update() {
	uint8_t brightness = SettingsHandler::getBrightness();

	if (ButtonHandler::getButton(BUTTON_VOL_UP)->hasChanged() == BUTTON_CHANGED_UP) {
		DisplayHandler::setBrightness(brightness + 1);
		SettingsHandler::setBrightness(brightness + 1);
	}

	if (ButtonHandler::getButton(BUTTON_VOL_DOWN)->hasChanged() == BUTTON_CHANGED_UP) {
		DisplayHandler::setBrightness(brightness - 1);	
		SettingsHandler::setBrightness(brightness - 1);
	}
}