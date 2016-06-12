#define DEBUG

// Buttons
#include "ButtonHandler.h"
#include "Button.h"

// States
#include "StateHandler.h"

void setup() {
  // Setup buttons
  ButtonHandler::setupButtons();

  // Setup states
  StateHandler::setupStates();
}

void loop() { 
  // Update buttons
  ButtonHandler::updateButtons();

  // Update states
  StateHandler::updateStates();
}
