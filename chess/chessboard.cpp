#include <Arduino.h>
#include "chessboard.h"
#include "chesspiece.h"

ChessBoard::ChessBoard() {
  lastChanged = 0;
}

void ChessBoard::setStartPosition() {
  this->setPiece(0,0,WHITE_ROOK);
  this->setPiece(1,0,WHITE_KNIGHT);
  this->setPiece(2,0,WHITE_BISHOP);
  this->setPiece(3,0,WHITE_QUEEN);
  this->setPiece(4,0,WHITE_KING);
  this->setPiece(5,0,WHITE_BISHOP);
  this->setPiece(6,0,WHITE_KNIGHT);
  this->setPiece(7,0,WHITE_ROOK);

  this->setPiece(0,7,BLACK_ROOK);
  this->setPiece(1,7,BLACK_KNIGHT);
  this->setPiece(2,7,BLACK_BISHOP);
  this->setPiece(3,7,BLACK_QUEEN);
  this->setPiece(4,7,BLACK_KING);
  this->setPiece(5,7,BLACK_BISHOP);
  this->setPiece(6,7,BLACK_KNIGHT);
  this->setPiece(7,7,BLACK_ROOK);

  for(int x=0;x<8;x++) {
    this->setPiece(x,1,WHITE_PAWN);
    this->setPiece(x,6,BLACK_PAWN);
  }
}

char ChessBoard::getPiece(int x, int y) {
  return pieces[y*8+x];
}

void ChessBoard::setPiece(int x, int y, char piece) {
  if(pieces[y*8+x] != piece) {
    pieces[y*8+x] = piece;
    lastChanged = millis();
  }
}


long ChessBoard::getLastChanged() {
  return lastChanged;
}

bool ChessBoard::isStartPosition() {
  for(int y=0;y<2;y++) {
    for(int x=0;x<8;x++) {
      char piece = this->getPiece(x, y);
      if(piece == EMPTY || !ChessPiece::isWhite(piece)) {
        return false;
      }
    }
  }
  for(int y=6;y<8;y++) {
    for(int x=0;x<8;x++) {
      char piece = this->getPiece(x, y);
      if(piece == EMPTY || ChessPiece::isWhite(piece)) {
        return false;
      }
    }
  }
  for(int y=2;y<6;y++) {
    for(int x=0;x<8;x++) {
      if(this->getPiece(x, y) != EMPTY) {
        return false;
      }
    }
  }
  return true;
}

const char* ChessBoard::getBoardArray() {
  return pieces;
}
