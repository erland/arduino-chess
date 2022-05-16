#ifndef CHESSDISPLAY_H
#define CHESSDISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

class ChessDisplay {
  private:
    Adafruit_ST7735* tft;
    int cell_size;
  public:
    ChessDisplay(Adafruit_ST7735* tft, int cell_size);
    void init();
    void refresh(const char* board);
};

#endif
