#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

class DisplayHandler {
  public:
    static void setupDisplay();
    static void setBrightness(uint8_t brightness);
    static uint8_t getBrightness();
    static void clearDisplay();

  private:
    DisplayHandler() {};
    
    static LiquidCrystal _display;
};

#endif // __DISPLAY_HANDLER_H__
