#include "chesspiece.h"

bool ChessPiece::isWhite(char piece) {
  if(piece == WHITE_PAWN || piece == WHITE_ROOK || piece == WHITE_KNIGHT || piece == WHITE_BISHOP || piece == WHITE_QUEEN || piece == WHITE_KING || piece == WHITE_UNKNOWN) {
    return true;
  }
  return false;
}

bool ChessPiece::isPawn(char piece) {
  return piece == WHITE_PAWN || piece == BLACK_PAWN;
}

bool ChessPiece::isRook(char piece) {
  return piece == WHITE_ROOK || piece == BLACK_ROOK;
}

bool ChessPiece::isKnight(char piece) {
  return piece == WHITE_KNIGHT || piece == BLACK_KNIGHT;
}

bool ChessPiece::isBishop(char piece) {
  return piece == WHITE_BISHOP || piece == BLACK_BISHOP;
}

bool ChessPiece::isQueen(char piece) {
  return piece == WHITE_QUEEN || piece == BLACK_QUEEN;
}

bool ChessPiece::isKing(char piece) {
  return piece == WHITE_KING || piece == BLACK_KING;
}
