#define DEBUG

#include "ButtonHandler.h"
#include "StateHandler.h"

void setup() {
  ButtonHandler::setupButtons();
  StateHandler::setupStates();
}

void loop() { 
  ButtonHandler::updateButtons();
  StateHandler::updateStates();
}
