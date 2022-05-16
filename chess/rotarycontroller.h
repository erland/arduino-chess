#ifndef ROTARYCONTROLLER_H
#define ROTARYCONTROLLER_H

#include <Arduino.h>

class RotaryController {
  private:
    int clkPin;
    int dtPin;
    int swPin;
    int value;
    uint8_t prevNextCode;
    uint16_t store;
    int8_t RotaryController::read_rotary();
  public:
    RotaryController(int clkPin, int dtPin, int swPin);
    void init();
    void refresh();
    int getValue();
};

#endif
