#ifndef CHESSCONTROLLER_H
#define CHESSCONTROLLER_H

#include "rotarycontroller.h"
#include "chessboard.h"
#include "chessdisplay.h"
#include "chesspieceselector.h"
#include "sensormatrix.h"
#include "Adafruit_LEDBackpack.h"

class ChessController : public RotaryControllerListener, public SensorMatrixListener, public ChessPieceSelectorListener {
  private:
    RotaryController* rotaryController;
    ChessPieceSelector* chessPieceSelector;
    ChessBoard* chessBoard;
    ChessDisplay* chessDisplay;
    SensorMatrix* sensorMatrix;
    Adafruit_8x16matrix *ledMatrix;
    char currentlyMoved;
    void enableLed(int x, int y);
    void activateCurrentlyPlacedLed(int index);
    void refreshLedMatrix();
  public:
    ChessController(RotaryController* rotaryController, SensorMatrix* sensorMatrix, ChessPieceSelector* chessPieceSelector, ChessBoard* chessBoard, ChessDisplay* chessDisplay, Adafruit_8x16matrix *ledMatrix);
    void init();
    virtual void chessPieceSelectorPieceSelected(char piece);
    virtual void sensorMatrixChanged(int x, int y, char oldValue, char newValue);
    virtual void rotaryControllerChangedRotation(int value, int offset);
    virtual void rotaryControllerButtonDown();
    virtual void rotaryControllerButtonUp();
};

#endif
