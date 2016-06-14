#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

class DisplayHandler {
  	public:
	    static void setup();
    	static void setBrightness(uint8_t brightness);
    	static void clearDisplay();

  	private:
    	static LiquidCrystal _display;
};

#endif // __DISPLAY_HANDLER_H__