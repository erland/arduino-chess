#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Adafruit_LEDBackpack.h"
#include <NoDelay.h>
#include "sensormatrix.h"
#include "chessboard.h"
#include "chessdisplay.h"
#include "chesspieceselector.h"
#include "rotarycontroller.h"
#include "chesscontroller.h"

#define TFT_RST   47
#define TFT_CS    48
#define TFT_DC    46
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


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

Adafruit_8x16matrix ledMatrix = Adafruit_8x16matrix();

noDelay ledBlinkRate(1000);
noDelay boardScanRate(100);


SensorMatrix sensorMatrix(25, &muxControlPins, &muxSignalPins, &muxSquareMapping);
RotaryController rotaryController(4, 3, 2);
ChessBoard chessBoard;
ChessDisplay chessDisplay(&tft, 12);
ChessPieceSelector chessPieceSelector(25, A4, A5, A6, A7, A8, A9);
ChessController chessController(&rotaryController, &sensorMatrix, &chessPieceSelector, &chessBoard, &chessDisplay, &ledMatrix);

long lastChanged = 0;
void setup() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);     // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.setRotation(1);
  ledMatrix.begin(0x70); 
  ledMatrix.setBrightness(1);

  rotaryController.init();
  chessPieceSelector.init();
  chessDisplay.init();
  sensorMatrix.init();
  chessController.init();
  
  Serial.println("");
  Serial.println("Ready to play");
  for ( int y=0;y<8;y++) {
    for ( int x=0;x<8;x++) {
      drawBlinkPixel(x,y,25);
    }
  }
  drawBlinkPixel(8,0,25);
  drawBlinkPixel(8,1,25);
  drawBlinkPixel(8,2,25);
  drawBlinkPixel(8,5,25);
  drawBlinkPixel(8,6,25);
  drawBlinkPixel(8,7,25);

  chessDisplay.refresh(chessBoard.getBoardArray());
}

void loop() {
  rotaryController.refresh();
  if(boardScanRate.update()) {
    sensorMatrix.refresh();
    long changed = chessBoard.getLastChanged();
    if(lastChanged != changed) {
      lastChanged = changed;
      chessDisplay.refresh(chessBoard.getBoardArray());
    }
    chessPieceSelector.refresh();
  }
}


void drawBlinkPixel(int x, int y, int rate) {
  drawPixel(x,y,1);
  ledMatrix.writeDisplay();
  delay(rate);
  drawPixel(x,y,0);
  ledMatrix.writeDisplay();
  delay(rate);
}

void drawPixel(int x, int y, int value) {
  ledMatrix.drawPixel(7-y, x, value);  
}
