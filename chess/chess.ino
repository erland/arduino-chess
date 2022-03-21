#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <NoDelay.h>

int muxControlPins[4][4] = {
  {30,31,32,33},
  {34,35,36,37},
  {38,39,40,41},
  {42,43,44,45}
};
int muxSignalPins[4] = {A0, A1, A2, A3};
int muxSquareMapping[64] = {19,18,17,16,15,11, 7, 3,
                            23,22,21,20,14,10, 6, 2,
                            27,26,25,24,13, 9, 5, 1,
                            31,30,29,28,12, 8, 4, 0,
                            35,34,33,32,48,52,56,60,
                            39,38,37,36,49,53,57,61,
                            43,42,41,40,50,54,58,62,
                            47,46,45,44,51,55,59,63};

#define EMPTY ' '
#define BLACK 'B'
#define WHITE 'W'

int defaultValue[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char markers[64] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

noDelay ledBlinkRate(1000);
noDelay boardScanRate(100);
noDelay resetTimer(5000);
noDelay debugRate(500);

void setup() {
  Serial.begin(115200);
  matrix.begin(0x70); 
  matrix.setBrightness(1);

  for(int muxNo=0;muxNo<4;muxNo++) {
    for(int sx=0;sx<4;sx++) {
      pinMode(muxControlPins[muxNo][sx], OUTPUT);
      digitalWrite(muxControlPins[muxNo][sx], LOW);
    }
    pinMode(muxSignalPins[muxNo], INPUT);
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
  Serial.println("");
  Serial.println("Ready to play");
  for ( int y=0;y<8;y++) {
    for ( int x=0;x<8;x++) {
      drawBlinkPixel(x,y,50);
    }
  }
  printBoard();
}

int lastPlaced = -1;
int lastRemoved = -1;

void loop() {
  if(boardScanRate.update()) {
    int oldLastPlaced = lastPlaced;
    int oldLastRemoved = lastRemoved;
    scanBoard();
    if(lastPlaced != oldLastPlaced || lastRemoved != oldLastRemoved) {
      printBoard();
    }
    /*
    if(debugRate.update()) {
      for ( int y=0;y<8;y++) {
        for (int x=0;x<8;x++) {
          Serial.print(readMux(y*8+x));
          if(x!=7) {
            Serial.print(", ");
          }
        }
        Serial.println("");
      }
      Serial.println("");
    }
    */
  }
}


void drawBlinkPixel(int x, int y, int rate) {
  drawPixel(x,y,1);
  matrix.writeDisplay();
  delay(rate);
  drawPixel(x,y,0);
  matrix.writeDisplay();
  delay(rate);
}

void scanBoard() {
  for ( int y=0;y<8;y++) {
    for (int x=0;x<8;x++) {
      int current = readMux(y*8+x);
      int offset = defaultValue[y*8+x];
      if(current<offset-20) {
        setMarker(x, y, WHITE);
      }else if(current>offset+20) {
        setMarker(x, y, BLACK);
      }else {
        setMarker(x,y, EMPTY);
      }
    }
  }
}

void printBoard() {
  matrix.clear(); 
  Serial.println("+--------+");
  for ( int y=0;y<8;y++) {
    Serial.print("|");
    for (int x=0;x<8;x++) {
      char m = getMarker(x, y);
      if(m == WHITE) {
        drawPixel(x,y,1);
        Serial.print("W");
      }else if(m == BLACK) {
        drawPixel(x,y,1);
        Serial.print("B");
      }else {
        Serial.print("_");
      }
    }
    Serial.println("|");
  }
  Serial.println("+--------+");
  Serial.println();
  matrix.writeDisplay();
}


bool isBoardEmpty() {
  for(int i=0;i<64;i++) {
    if(markers[i]!=EMPTY) {
      return false;
    }
  }
  return true;
}

char getMarker(int x, int y) {
  return markers[y*8+x];
}

int setMarker(int x, int y, char value) {
  if(markers[y*8+x] == EMPTY && value != EMPTY) {
    Serial.print("Placed ");
    Serial.print(value);
    Serial.print(" at ");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    lastPlaced = y*8+x;
    lastRemoved = -1;
  }else if(markers[y*8+x] != EMPTY && value == EMPTY) {
    Serial.print("Removed ");
    Serial.print(markers[y*8+x]);
    Serial.print(" from ");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    lastRemoved = y*8+x;
    lastPlaced = -1;
  }else if(markers[y*8+x] != EMPTY && markers[y*8+x] != value) {
    Serial.print("Replaced ");
    Serial.print(markers[y*8+x]);
    Serial.print(" with ");
    Serial.print(value);
    Serial.print(" at ");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    lastPlaced = y*8+x;
    lastRemoved = y*8+x;
  }
  markers[y*8+x] = value;
}

int markerCount(char markerType) {
  int count = 0;
  for(int i=0;i<64;i++) {
    if(markers[i] == markerType) {
      count++;
    }
  }
  return count;
}

void drawPixel(int x, int y, int value) {
  matrix.drawPixel(7-y, x, value);  
}

int readMux(int channel) {
  int muxIndex = muxSquareMapping[channel];
  int muxNo = muxIndex/16;
  int muxChannel = muxIndex%16;
  int controlPins[] = {muxControlPins[muxNo][3], muxControlPins[muxNo][2], muxControlPins[muxNo][1], muxControlPins[muxNo][0]};
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
  int val = analogRead(muxSignalPins[muxNo]);
  //Serial.print("Reading=");
  //Serial.println(val);
  return val;
}
