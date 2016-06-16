#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

#define CHAR_FULL_BLOCK		255

#define CHAR_HALF_BLOCK		0

class DisplayHandler {
  	public:
	    static void setup();
    	static void setBrightness(uint8_t brightness);
    	static void clearDisplay();
    	static void setCursor(uint8_t row, uint8_t col);
    	static void print(const char str[]);
    	static void write(uint8_t chr);

  	private:
    	static LiquidCrystal _display;
};

#endif // __DISPLAY_HANDLER_H__