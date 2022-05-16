#include "chessdisplay.h"

ChessDisplay::ChessDisplay(Adafruit_ST7735* tft, int cell_size) {
  this->tft = tft;
  this->cell_size = cell_size;
}

void ChessDisplay::init() {
  tft->fillScreen(ST7735_BLACK);  // fill screen with black color
  for( int x=0;x<8;x++) {
    for(int y=0;y<8;y++) {
      if((x%2==0 && y%2==0) || (x%2==1 && y%2==1)) {
        tft->fillRect(10+x*cell_size,10+y*cell_size,cell_size,cell_size, 0xDCA9);
      }else {
        tft->fillRect(10+x*cell_size,10+y*cell_size,cell_size,cell_size, 0x8A62);
      }
    }
  }
  tft->setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft->setTextSize(1);
  tft->setCursor(10+8*cell_size+10, 20);
  tft->print("Schack");
  tft->setCursor(10+8*cell_size+10, 40);
  //tft->print(rotary_value);

}

void ChessDisplay::refresh(const char* board) {
  tft->fillScreen(ST7735_BLACK);  // fill screen with black color
  for( int x=0;x<8;x++) {
    for(int y=0;y<8;y++) {
      if((x%2==0 && y%2==0) || (x%2==1 && y%2==1)) {
        tft->fillRect(10+x*cell_size,10+y*cell_size,cell_size,cell_size, 0xDCA9);
      }else {
        tft->fillRect(10+x*cell_size,10+y*cell_size,cell_size,cell_size, 0x8A62);
      }
    }
  }
  tft->setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft->setTextSize(1);
  for(int y=0;y<8;y++) {
    for(int x=0;x<8;x++) {
      tft->setCursor(10+3+x*cell_size,10+3+(7-y)*cell_size);
      char cell = board[y*8+x];
      if(cell == 'P') {
        tft->setTextColor(ST7735_WHITE);
        tft->print('B');
      }else if(cell == 'R') {
        tft->setTextColor(ST7735_WHITE);
        tft->print('T');
      }else if(cell == 'N') {
        tft->setTextColor(ST7735_WHITE);
        tft->print('S');
      }else if(cell == 'B') {
        tft->setTextColor(ST7735_WHITE);
        tft->print('L');
      }else if(cell == 'Q') {
        tft->setTextColor(ST7735_WHITE);
        tft->print('D');
      }else if(cell == 'K') {
        tft->setTextColor(ST7735_WHITE);
        tft->print('K');
      }else if(cell == 'p') {
        tft->setTextColor(ST7735_BLACK);
        tft->print('B');
      }else if(cell == 'r') {
        tft->setTextColor(ST7735_BLACK);
        tft->print('T');
      }else if(cell == 'n') {
        tft->setTextColor(ST7735_BLACK);
        tft->print('S');
      }else if(cell == 'b') {
        tft->setTextColor(ST7735_BLACK);
        tft->print('L');
      }else if(cell == 'q') {
        tft->setTextColor(ST7735_BLACK);
        tft->print('D');
      }else if(cell == 'k') {
        tft->setTextColor(ST7735_BLACK);
        tft->print('K');
      }
    }
  }
}
