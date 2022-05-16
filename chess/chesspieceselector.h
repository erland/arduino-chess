#ifndef CHESSPIECESELECTOR_H
#define CHESSPIECESELECTOR_H

#include "chessboard.h"

class ChessPieceSelector {
  private:
    int defaultValue[6];
    int pins[6];
    int sensitivity;
    ChessBoard *chessBoard;
  public:
    ChessPieceSelector(int sensitivity, ChessBoard* chessBoard, int pawnPin, int rookPin, int knightPin, int bishopPin, int queenPin, int kingPin);
    void init();
    void refresh();
    void scan(int defaultValue, int pin, char whitePiece, char blackPiece);
};

#endif
