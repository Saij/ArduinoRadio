#include "Debug.h"
#include "DisplayHandler.h"
#include "SettingsHandler.h"
#include "PowerHandler.h"
#include "ButtonHandler.h"
#include "StateHandler.h"
#include "TunerHandler.h"

void setup() {
	setupDebug();

	SettingsHandler::setup();
	DisplayHandler::setup();
	TunerHandler::setup();
	ButtonHandler::setup();
	StateHandler::setup();
	PowerHandler::setup();
}

void loop() {
	SettingsHandler::update();
	ButtonHandler::update();
	TunerHandler::update();
	StateHandler::update();
}