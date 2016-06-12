#include "Button.h"

#define DEBOUNCE_DELAY  50
#define CHANGE_DELAY    50

void Button::setState(uint8_t state) {
  if (state != this->_lastButtonState) {
    this->_lastDebounceTime = millis();
  }

  if ((millis() - this->_lastDebounceTime) > DEBOUNCE_DELAY) {
    if (state != this->_buttonState) {
      this->_buttonState = state;
      this->_lastChangeTime = millis();
      this->_hasChanged = true;
    } else {
      this->_hasChanged = false;
    }
  }

  this->_lastButtonState = state;
}

uint8_t Button::getState() {
  return this->_buttonState;
}

uint8_t Button::hasChanged() {
  if (this->_hasChanged && this->isUp()) {
    return BUTTON_CHANGED_UP;
  }

  if (this->_hasChanged && this->isDown()) {
    return BUTTON_CHANGED_DOWN;
  }
  
  return BUTTON_CHANGED_NOT;
}

bool Button::isUp() {
  return this->_buttonState == BUTTON_STATE_UP;
}

bool Button::isDown() {
  return this->_buttonState == BUTTON_STATE_DOWN;
}

