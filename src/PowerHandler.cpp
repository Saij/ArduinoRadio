#include "PowerHandler.h"
#include "Debug.h"
#include <Arduino.h>

#define PIN_POWER 7

void PowerHandler::setup() {
  	debugPrintf(F("Initialize Power"));
  
  	pinMode(PIN_POWER, OUTPUT);
  	digitalWrite(PIN_POWER, HIGH);
}