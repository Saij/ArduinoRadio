#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

#define BUTTON_CHANGED_NOT  0x0
#define BUTTON_CHANGED_UP   0x1
#define BUTTON_CHANGED_DOWN 0x2

#define BUTTON_STATE_UP     LOW
#define BUTTON_STATE_DOWN   HIGH

class Button {
  public:
    Button() {};

    void setState(uint8_t state);
    uint8_t getState();
    uint8_t hasChanged();

    bool isDown();
    bool isUp();
  
  private:
    uint8_t _buttonState = LOW;
    uint8_t _lastButtonState = LOW;
    bool _hasChanged = false;
    unsigned long _lastDebounceTime = 0;
    unsigned long _lastChangeTime = 0;
};

#endif // __BUTTON_H__
