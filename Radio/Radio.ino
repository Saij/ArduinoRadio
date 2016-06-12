#include "Debug.h"
#include "DisplayHandler.h"
#include "SettingsHandler.h"
#include "PowerHandler.h"
#include "ButtonHandler.h"
#include "StateHandler.h"

void setup() {
  setupDebug();

  SettingsHandler::setupSettings();
  PowerHandler::setupPower();
  DisplayHandler::setupDisplay();
  ButtonHandler::setupButtons();
  StateHandler::setupStates();
}

void loop() { 
  ButtonHandler::updateButtons();
  StateHandler::updateStates();
}
