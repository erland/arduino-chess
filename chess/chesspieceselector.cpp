#include <Arduino.h>
#include "chesspieceselector.h"
#include "chesspiece.h"

ChessPieceSelector::ChessPieceSelector(int sensitivity, int pawnPin, int rookPin, int knightPin, int bishopPin, int queenPin, int kingPin) {
  pins[0] = pawnPin;
  pins[1] = rookPin;
  pins[2] = knightPin;
  pins[3] = bishopPin;
  pins[4] = queenPin;
  pins[5] = kingPin;
  this->sensitivity = sensitivity;
  this->listener = NULL;
}

void ChessPieceSelector::setListener(ChessPieceSelectorListener* listener) {
  this->listener = listener;
}

void ChessPieceSelector::init() {
  for(int i=0;i<6;i++) {
    pinMode(pins[i], INPUT);
    defaultValue[i] = analogRead(pins[i]);
    Serial.print(pins[i]);Serial.print("=");Serial.println(defaultValue[i]);
  }
}

void ChessPieceSelector::refresh() {
  if(listener != NULL) {
    scan(defaultValue[0], pins[0], WHITE_PAWN, BLACK_PAWN);
    scan(defaultValue[1], pins[1], WHITE_ROOK, BLACK_ROOK);
    scan(defaultValue[2], pins[2], WHITE_KNIGHT, BLACK_KNIGHT);
    scan(defaultValue[3], pins[3], WHITE_BISHOP, BLACK_BISHOP);
    scan(defaultValue[4], pins[4], WHITE_QUEEN, BLACK_QUEEN);
    scan(defaultValue[5], pins[5], WHITE_KING, BLACK_KING);
  }
}

void ChessPieceSelector::scan(int defaultValue, int pin, char whitePiece, char blackPiece) {
  int current = analogRead(pin);
  if(current<defaultValue-sensitivity) {
    listener->chessPieceSelectorPieceSelected(blackPiece);
  }else if(current>defaultValue+sensitivity) {
    listener->chessPieceSelectorPieceSelected(whitePiece);
  }
}
