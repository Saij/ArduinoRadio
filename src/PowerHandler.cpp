#include "PowerHandler.h"
#include "Debug.h"
#include <Arduino.h>

#define PIN_POWER A3

void PowerHandler::setup() {
  	debugPrintf(F("Initialize Power"));
  
  	pinMode(PIN_POWER, OUTPUT);
  	digitalWrite(PIN_POWER, HIGH);
}