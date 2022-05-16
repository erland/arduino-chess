#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Adafruit_LEDBackpack.h"
#include "sensormatrix.h"

#define EMPTY ' '
#define WHITE_UNKNOWN '?'
#define BLACK_UNKNOWN '%'
#define WHITE_PAWN 'P'
#define BLACK_PAWN 'p'
#define WHITE_ROOK 'R'
#define BLACK_ROOK 'r'
#define WHITE_KNIGHT 'N'
#define BLACK_KNIGHT 'n'
#define WHITE_BISHOP 'B'
#define BLACK_BISHOP 'b'
#define WHITE_QUEEN 'Q'
#define BLACK_QUEEN 'q'
#define WHITE_KING 'K'
#define BLACK_KING 'k'


class ChessBoard : public SensorMatrixListener {
  private: 
    SensorMatrix *sensorMatrix;
    Adafruit_8x16matrix *ledMatrix;
    char pieces[64];
    char currentlyMoved;
    bool isWhite(char piece);
    void enableLed(int x, int y);
    void activateCurrentlyPlacedLed(int index);
    void refreshLedMatrix();
    long lastChanged;
  public:
    ChessBoard(SensorMatrix *sensorMatrix, Adafruit_8x16matrix *ledMatrix);
    virtual void sensorMatrixChanged(int x, int y, char oldValue, char newValue);
    char getPiece(int x, int y);
    long getLastChanged();
    //char getCurrentlyMoved();
    //void setCurrentlyMoved(int x, int y);
    void setCurrentlyMoved(char pieceType);
    //void clearCurrentlyMoved();
    const char* getBoardArray();
    char* getBoardFENNotation();
};

#endif
