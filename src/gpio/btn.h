#ifndef BTN_H
#define BTN_H

#include <Arduino.h>

class Btn {
  public:
    Btn(uint8_t pin, uint16_t debounceDelay = 50);

    void init();
    void update();

    bool wasPressed();
    bool isPressed();

  private:
    uint8_t _pin;
    uint16_t _debounceDelay;
    
    bool _lastStableState;
    bool _currentReading;
    bool _wasPressedFlag;

    unsigned long _lastDebounceTime;
};

#endif
