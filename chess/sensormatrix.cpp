#include <Arduino.h>
#include "sensormatrix.h"

SensorMatrix::SensorMatrix(int sensitivity, int (*muxControlPins)[4][4], int (*muxSignalPins)[4], int (*muxSquareMapping)[64]) {
  this->sensitivity = sensitivity;
  this->muxControlPins = muxControlPins;
  this->muxSignalPins = muxSignalPins;
  this->muxSquareMapping = muxSquareMapping;
  for (int i=0;i<64;i++) {
    defaultValue[i] = 0;
    sensor_states[i] = SENSOR_STATE_EMPTY;
  }
  this->listener = NULL;
}

void SensorMatrix::setListener(SensorMatrixListener* listener) {
  this->listener = listener;
}

void SensorMatrix::init() {
  for(int muxNo=0;muxNo<4;muxNo++) {
    for(int sx=0;sx<4;sx++) {
      pinMode((*muxControlPins)[muxNo][sx], OUTPUT);
      digitalWrite((*muxControlPins)[muxNo][sx], LOW);
    }
    pinMode((*muxSignalPins)[muxNo], INPUT);
  }
  Serial.println("Initializing with: ");
  for ( int y=0;y<8;y++) {
    for (int x=0;x<8;x++) {
      defaultValue[y*8+x] = readMux(y*8+x);
      
      Serial.print(defaultValue[y*8+x]);
      if(x!=7) {
        Serial.print(", ");
      }
    }
    Serial.println("");
  }
}

void SensorMatrix::refresh() {
  for ( int y=0;y<8;y++) {
    for (int x=0;x<8;x++) {
      int current = readMux(y*8+x);
      int offset = defaultValue[y*8+x];
      if(current<offset-sensitivity) {
        setSensorState(x, y, SENSOR_STATE_BLACK);
      }else if(current>offset+sensitivity) {
        setSensorState(x, y, SENSOR_STATE_WHITE);
      }else {
        setSensorState(x, y, SENSOR_STATE_EMPTY);
      }
    }
  }
}

char SensorMatrix::getSensorState(int x, int y) {
  return sensor_states[y*8+x];
}

void SensorMatrix::setSensorState(int x, int y, char value) {
  int oldValue = sensor_states[y*8+x];
  if(oldValue != value) {
    if(sensor_states[y*8+x] == SENSOR_STATE_EMPTY && value != SENSOR_STATE_EMPTY) {
      Serial.print("Placed ");
      Serial.print(value);
      Serial.print(" at ");
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);
      //lastPlaced = y*8+x;
      //lastRemoved = -1;
    }else if(sensor_states[y*8+x] != SENSOR_STATE_EMPTY && value == SENSOR_STATE_EMPTY) {
      Serial.print("Removed ");
      Serial.print(sensor_states[y*8+x]);
      Serial.print(" from ");
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);
      //lastRemoved = y*8+x;
      //lastPlaced = -1;
    }else if(sensor_states[y*8+x] != SENSOR_STATE_EMPTY && sensor_states[y*8+x] != value) {
      Serial.print("Replaced ");
      Serial.print(sensor_states[y*8+x]);
      Serial.print(" with ");
      Serial.print(value);
      Serial.print(" at ");
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);
      //lastPlaced = y*8+x;
      //lastRemoved = y*8+x;
    }
    sensor_states[y*8+x] = value;
  
    if(listener != NULL) {
      listener->sensorMatrixChanged(x, y, oldValue, value);
    }
  }
}

int SensorMatrix::readMux(int channel) {
  int muxIndex = (*muxSquareMapping)[channel];
  int muxNo = muxIndex/16;
  int muxChannel = muxIndex%16;
  int controlPins[] = {(*muxControlPins)[muxNo][3], (*muxControlPins)[muxNo][2], (*muxControlPins)[muxNo][1], (*muxControlPins)[muxNo][0]};
  const int muxChannelSettings[16][4] = {
    {0,0,0,0},
    {0,0,0,1},
    {0,0,1,0},
    {0,0,1,1},
    {0,1,0,0},
    {0,1,0,1},
    {0,1,1,0},
    {0,1,1,1},
    {1,0,0,0},
    {1,0,0,1},
    {1,0,1,0},
    {1,0,1,1},
    {1,1,0,0},
    {1,1,0,1},
    {1,1,1,0},
    {1,1,1,1}
  };

  //Serial.print("Configuring mux(");
  //Serial.print(muxNo);
  //Serial.print(") with ");
  for (int i = 0;i<4;i++) {
    //Serial.print(controlPins[i]);
    //Serial.print("=");
    //Serial.print(muxChannelSettings[muxChannel][i]);
    //Serial.print(", ");
    digitalWrite(controlPins[i], muxChannelSettings[muxChannel][i]);
  }
  //Serial.println("");
  //delay(1);
  int val = analogRead((*muxSignalPins)[muxNo]);
  //Serial.print("Reading=");
  //Serial.println(val);
  return val;
}
