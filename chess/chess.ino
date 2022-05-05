#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Adafruit_LEDBackpack.h"
#include <NoDelay.h>

#define TFT_RST   47
#define TFT_CS    48
#define TFT_DC    46
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define ROTARY_CLK 4
#define ROTARY_DT 3
#define ROTARY_SW 2

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
int placementDefaultValue[6] = {0,0,0,0,0,0};
char placementMarker[6] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

noDelay ledBlinkRate(1000);
noDelay boardScanRate(100);
noDelay resetTimer(5000);
noDelay debugRate(500);

int rotary_value = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ROTARY_CLK, INPUT_PULLUP);
  pinMode(ROTARY_DT, INPUT_PULLUP);
  tft.initR(INITR_BLACKTAB);     // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.setRotation(1);
  matrix.begin(0x70); 
  matrix.setBrightness(1);

  for(int muxNo=0;muxNo<4;muxNo++) {
    for(int sx=0;sx<4;sx++) {
      pinMode(muxControlPins[muxNo][sx], OUTPUT);
      digitalWrite(muxControlPins[muxNo][sx], LOW);
    }
    pinMode(muxSignalPins[muxNo], INPUT);
  }
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);

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
  placementDefaultValue[0] = analogRead(A4);
  Serial.print("A4=");Serial.println(placementDefaultValue[0]);
  placementDefaultValue[1] = analogRead(A5);
  Serial.print("A5=");Serial.println(placementDefaultValue[1]);
  placementDefaultValue[2] = analogRead(A6);
  Serial.print("A6=");Serial.println(placementDefaultValue[2]);
  placementDefaultValue[3] = analogRead(A7);
  Serial.print("A7=");Serial.println(placementDefaultValue[3]);
  placementDefaultValue[4] = analogRead(A8);
  Serial.print("A8=");Serial.println(placementDefaultValue[4]);
  placementDefaultValue[5] = analogRead(A9);
  Serial.print("A9=");Serial.println(placementDefaultValue[5]);
  
  Serial.println("");
  Serial.println("Ready to play");
  for ( int y=0;y<8;y++) {
    for ( int x=0;x<8;x++) {
      drawBlinkPixel(x,y,50);
    }
  }
  drawBlinkPixel(8,0,50);
  drawBlinkPixel(8,1,50);
  drawBlinkPixel(8,2,50);
  drawBlinkPixel(8,5,50);
  drawBlinkPixel(8,6,50);
  drawBlinkPixel(8,7,50);
  printBoard();
  refreshDisplay();
}

int lastPlaced = -1;
int lastRemoved = -1;
int lastPlacedPlacementMarker = -1;
int lastRemovedPlacementMarker = -1;

void loop() {
  int rotary_offset = read_rotary();
  if(rotary_offset!=0) {
    rotary_value = rotary_value + rotary_offset;
    refreshDisplay();
  }
  if(boardScanRate.update()) {
    int oldLastPlaced = lastPlaced;
    int oldLastRemoved = lastRemoved;
    int oldLastPlacedPlacementMarker = lastPlacedPlacementMarker;
    int oldLastRemovedPlacementMarker = lastRemovedPlacementMarker;
    scanBoard();
    scanPlacementMarker(0, A4);
    scanPlacementMarker(1, A5);
    scanPlacementMarker(2, A6);
    scanPlacementMarker(3, A7);
    scanPlacementMarker(4, A8);
    scanPlacementMarker(5, A9);
    if(lastPlaced != oldLastPlaced || lastRemoved != oldLastRemoved || lastPlacedPlacementMarker != oldLastPlacedPlacementMarker || lastRemovedPlacementMarker != oldLastRemovedPlacementMarker) {
      printBoard();
      refreshDisplay();
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

void refreshDisplay() {
  int cell_size = 12;
  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  for( int x=0;x<9;x++) {
    for(int y=0;y<9;y++) {
      tft.drawLine(10,10+y*cell_size,10+8*cell_size,10+y*cell_size, ST7735_WHITE);
    }
    tft.drawLine(10+x*cell_size,10,10+x*cell_size,10+8*cell_size, ST7735_WHITE);
  }
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10+8*cell_size+10, 20);
  tft.print("Schack");
  tft.setCursor(10+8*cell_size+10, 40);
  tft.print(rotary_value);
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

char scanPlacementMarker(int index, int pin) {
  int offset = placementDefaultValue[index];
  int current = analogRead(pin);
  if(current<offset-20) {
    refreshLastPlacedPlacementMarker(index, placementMarker[index], WHITE);
    placementMarker[index] = WHITE;
  }else if(current>offset+20) {
    refreshLastPlacedPlacementMarker(index, placementMarker[index], BLACK);
    placementMarker[index] = BLACK;
  }else {
    refreshLastPlacedPlacementMarker(index, placementMarker[index], EMPTY);
    placementMarker[index] = EMPTY;
  }
}

void refreshLastPlacedPlacementMarker(int index, char oldValue, char newValue) {
  if(oldValue != newValue) {
    if(newValue != EMPTY) {
      lastPlacedPlacementMarker = index;
    }
    if(oldValue != EMPTY) {
      lastRemovedPlacementMarker = index;
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
  for(int i=0;i<6;i++) {
    printPlacementMarker(i);
  }
  Serial.println();
  matrix.writeDisplay();
}

void printPlacementMarker(int index) {
  if(placementMarker[index] != EMPTY) {
    Serial.print(placementMarker[index]);
    if(index<3) {
      drawPixel(8, index, 1);
    }else {
      drawPixel(8, 2+index, 1);
    }
  }else {
    Serial.print("-");
  }
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

static uint8_t prevNextCode = 0;
static uint16_t store=0;
// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(ROTARY_DT)) prevNextCode |= 0x02;
  if (digitalRead(ROTARY_CLK)) prevNextCode |= 0x01;
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
