#include <Arduino.h>
#include "chessboard.h"

ChessBoard::ChessBoard() {
  lastChanged = 0;
}

char ChessBoard::getPiece(int x, int y) {
  return pieces[y*8+x];
}

void ChessBoard::setPiece(int x, int y, char piece) {
  pieces[y*8+x] = piece;
  lastChanged = millis();
}


long ChessBoard::getLastChanged() {
  return lastChanged;
}

const char* ChessBoard::getBoardArray() {
  return pieces;
}
