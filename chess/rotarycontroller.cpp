#include "rotarycontroller.h"

RotaryController::RotaryController(int clkPin, int dtPin, int swPin) {
  this->clkPin = clkPin;
  this->dtPin = dtPin;
  this->swPin = swPin;
  prevNextCode = 0;
  store=0;
}

void RotaryController::init() {
  pinMode(clkPin, INPUT_PULLUP);
  pinMode(dtPin, INPUT_PULLUP);
}

void RotaryController::refresh() {
  int offset = read_rotary();
  if(offset!=0) {
    value = value + offset;
    Serial.print(value);
  }
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t RotaryController::read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(dtPin)) prevNextCode |= 0x02;
  if (digitalRead(clkPin)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}
