#include "Debug.h"
#include "PowerHandler.h"
#include "ButtonHandler.h"
#include "StateHandler.h"

void setup() {
  setupDebug();

  PowerHandler::setupPower();
  ButtonHandler::setupButtons();
  StateHandler::setupStates();
}

void loop() { 
  ButtonHandler::updateButtons();
  StateHandler::updateStates();
}
