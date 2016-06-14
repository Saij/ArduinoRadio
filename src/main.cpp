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
  	PowerHandler::setup();
  	DisplayHandler::setup();
  	TunerHandler::setup();
  	ButtonHandler::setup();
  	StateHandler::setup();
}

void loop() { 
	SettingsHandler::update();
  	ButtonHandler::update();
  	StateHandler::update();
}