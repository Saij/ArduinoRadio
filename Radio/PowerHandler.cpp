#include "PowerHandler.h"

#include <Arduino.h>

#define PIN_POWER A3

void PowerHandler::setupPower() {
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, HIGH);
}

