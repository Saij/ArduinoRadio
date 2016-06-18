#include "DisplayHandler.h"
#include "Debug.h"
#include "SettingsHandler.h"
#include <Arduino.h>

#define PIN_BRIGHTNESS      5
#define PIN_DISPLAY_RS      8
#define PIN_DISPLAY_ENABLE  9
#define PIN_DISPLAY_DATA1   10
#define PIN_DISPLAY_DATA2   11
#define PIN_DISPLAY_DATA3   12
#define PIN_DISPLAY_DATA4   13

#define DISPLAY_ROWS 4
#define DISPLAY_COLS 16

LiquidCrystal DisplayHandler::_display(PIN_DISPLAY_RS, PIN_DISPLAY_ENABLE, PIN_DISPLAY_DATA1, PIN_DISPLAY_DATA2, PIN_DISPLAY_DATA3, PIN_DISPLAY_DATA4);

void DisplayHandler::setup() {
  	debugPrintf(F("Initialize Display"));

  	pinMode(PIN_BRIGHTNESS, OUTPUT);
  	analogWrite(PIN_BRIGHTNESS, SettingsHandler::getBrightness());

  	DisplayHandler::_display.begin(DISPLAY_COLS, DISPLAY_ROWS);

  	// Adding chars
	uint8_t halfBar[8] = {
		B11100,
		B11000,
		B11100,
		B11000,
		B11100,
		B11000,
		B11100,
		B11000,
	};

	DisplayHandler::_display.createChar(CHAR_HALF_BLOCK, halfBar);
}

void DisplayHandler::setBrightness(uint8_t brightness) {
	analogWrite(PIN_BRIGHTNESS, brightness);
}

void DisplayHandler::clearDisplay() {
  	DisplayHandler::_display.clear();
}

void DisplayHandler::setCursor(uint8_t row, uint8_t col) {
	DisplayHandler::_display.setCursor(col, row);
}

void DisplayHandler::print(const char str[]) {
	DisplayHandler::_display.print(str);
}

void DisplayHandler::write(uint8_t chr) {
	DisplayHandler::_display.write(chr);
}