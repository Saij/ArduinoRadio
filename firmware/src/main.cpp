#include "Debug.h"
#include "DisplayHandler.h"
#include "SettingsHandler.h"
#include "PowerHandler.h"
#include "ButtonHandler.h"
#include "StateHandler.h"
#include "TunerHandler.h"

void setup() {
  	setupDebug();

  	SettingsHandler::setupSettings();
  	PowerHandler::setupPower();
  	DisplayHandler::setupDisplay();
  	TunerHandler::setupTuner();
  	ButtonHandler::setup();
  	StateHandler::setupStates();
}

void loop() { 
  	ButtonHandler::update();
  	StateHandler::updateStates();
}