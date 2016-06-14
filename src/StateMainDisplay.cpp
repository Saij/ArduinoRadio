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
}