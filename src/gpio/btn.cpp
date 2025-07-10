#include "btn.h"

Btn::Btn(uint8_t pin, uint16_t debounceDelay) {
  _pin = pin;
  _debounceDelay = debounceDelay;
  _lastStableState = HIGH;
  _currentReading = HIGH;
  _wasPressedFlag = false;
  _lastDebounceTime = 0;
}

void Btn::init() {
  pinMode(_pin, INPUT_PULLUP);
  _lastStableState = digitalRead(_pin);
}

void Btn::update() {
  bool reading = digitalRead(_pin);

  if (reading != _currentReading) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    if (reading != _lastStableState) {
      _lastStableState = reading;

      if (_lastStableState == LOW) {
        _wasPressedFlag = true;
      }
    }
  }

  _currentReading = reading;
}

bool Btn::wasPressed() {
  if (_wasPressedFlag) {
    _wasPressedFlag = false;
    return true;
  }
  return false;
}

bool Btn::isPressed() {
  return (_lastStableState == LOW);
}
