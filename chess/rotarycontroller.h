#ifndef ROTARYCONTROLLER_H
#define ROTARYCONTROLLER_H

#include <Arduino.h>

class RotaryControllerListener {
  public:
    virtual void rotaryControllerChangedRotation(int value, int offset) = 0;
    virtual void rotaryControllerButtonDown() = 0;
    virtual void rotaryControllerButtonUp() = 0;
};

class RotaryController {
  private:
    int clkPin;
    int dtPin;
    int swPin;
    int value;
    uint8_t prevNextCode;
    uint16_t store;
    int8_t RotaryController::read_rotary();
    RotaryControllerListener* listener;
  public:
    RotaryController(int clkPin, int dtPin, int swPin);
    void setListener(RotaryControllerListener* listener);
    void init();
    void refresh();
    int getValue();
};

#endif
