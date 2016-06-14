#include "DisplayHandler.h"
#include "Debug.h"
#include "SettingsHandler.h"
#include <Arduino.h>

#define PIN_BRIGHTNESS      5
#define PIN_DISPLAY_RS      A1
#define PIN_DISPLAY_ENABLE  9
#define PIN_DISPLAY_DATA1   8
#define PIN_DISPLAY_DATA2   7
#define PIN_DISPLAY_DATA3   6
#define PIN_DISPLAY_DATA4   10

#define DISPLAY_ROWS 4
#define DISPLAY_COLS 16

LiquidCrystal DisplayHandler::_display(PIN_DISPLAY_RS, PIN_DISPLAY_ENABLE, PIN_DISPLAY_DATA1, PIN_DISPLAY_DATA2, PIN_DISPLAY_DATA3, PIN_DISPLAY_DATA4);

void DisplayHandler::setup() {
  	debugPrintf(F("Initialize Display"));

  	pinMode(PIN_BRIGHTNESS, OUTPUT);
  	analogWrite(PIN_BRIGHTNESS, SettingsHandler::getBrightness());

  	DisplayHandler::_display.begin(DISPLAY_COLS, DISPLAY_ROWS);
}

void DisplayHandler::setBrightness(uint8_t brightness) {
	analogWrite(PIN_BRIGHTNESS, brightness);
}

void DisplayHandler::clearDisplay() {
  	DisplayHandler::_display.clear();
} 