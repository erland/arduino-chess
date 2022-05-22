#include <Arduino.h>
#include "chessrules.h"
#include "chesspiece.h"

bool ChessRules::isValidKingMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  if(abs(fromX-toX)<=1 && abs(fromY-toY)<=1 && (abs(fromX-toX)!=0 || abs(fromY-toY)!=0)) {
    char toPiece = chessBoard->getPiece(toX, toY);
    if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
      return true;
    }
  }
  return false;
}
bool ChessRules::isValidQueenMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  return isValidBishopMove(chessBoard, piece, fromX, fromY, toX, toY) || isValidRookMove(chessBoard, piece, fromX, fromY, toX, toY);
}
bool ChessRules::isValidBishopMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  if(abs(fromX-toX)==abs(fromY-toY)) {
    if(fromX>toX) {
      for(int offset=1;fromX-offset>toX;offset++) {
        if(fromY>toY) {
          if(chessBoard->getPiece(fromX-offset,fromY-offset)!=EMPTY) {
            return false;
          }
        }else {
          if(chessBoard->getPiece(fromX-offset,fromY+offset)!=EMPTY) {
            return false;
          }
        }
      }
    }else {
      for(int offset=1;fromX+offset<toX;offset++) {
        if(fromY>toY) {
          if(chessBoard->getPiece(fromX+offset,fromY-offset)!=EMPTY) {
            return false;
          }
        }else {
          if(chessBoard->getPiece(fromX+offset,fromY+offset)!=EMPTY) {
            return false;
          }
        }
      }
    }
    char toPiece = chessBoard->getPiece(toX, toY);
    if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
      return true;
    }
  }
  return false;
}

bool ChessRules::isValidKnightMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  if((abs(fromX-toX)==2 && abs(fromY-toY)==1) || (abs(fromX-toX)==1 && abs(fromY-toY)==2)) {
    char toPiece = chessBoard->getPiece(toX, toY);
    if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
      return true;
    }
  }
  return false;
}

bool ChessRules::isValidRookMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  if(fromX==toX || fromY==toY) {
    if(fromX>toX) {
      for(int i=1;toX+i<fromX;i++) {
        if(chessBoard->getPiece(toX+i,toY) != EMPTY) {
          return false;
        }
      }
      char toPiece = chessBoard->getPiece(toX, toY);
      if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
        return true;
      }
    }else if(fromX<toX) {
      for(int i=1;toX-i>fromX;i++) {
        if(chessBoard->getPiece(toX-i,toY) != EMPTY) {
          return false;
        }
      }
      char toPiece = chessBoard->getPiece(toX, toY);
      if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
        return true;
      }
    }else if(fromY>toY) {
      for(int i=1;toY+i<fromY;i++) {
        if(chessBoard->getPiece(toX,toY+i) != EMPTY) {
          return false;
        }
      }
      char toPiece = chessBoard->getPiece(toX, toY);
      if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
        return true;
      }
    }else if(fromY<toY) {
      for(int i=1;toY-i>fromY;i++) {
        if(chessBoard->getPiece(toX,toY-i) != EMPTY) {
          return false;
        }
      }
      char toPiece = chessBoard->getPiece(toX, toY);
      if(toPiece == EMPTY || ChessPiece::isBlack(toPiece) != ChessPiece::isBlack(piece)) {
        return true;
      }
    }
  }
  return false;
}

bool ChessRules::isValidPawnMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  if(ChessPiece::isBlack(piece)) {
    if(fromX==toX) {
      if(fromY==toY+1 && chessBoard->getPiece(toX, toY) == EMPTY) {
        return true;
      }else if(fromY==toY+2 && fromY==6 && chessBoard->getPiece(toX, toY) == EMPTY && chessBoard->getPiece(toX, toY+1) == EMPTY) {
        return true;
      }
    }else if(fromX==toX-1 || fromX==toX+1) {
      if(fromY==toY+1 && ChessPiece::isWhite(chessBoard->getPiece(toX, toY))) {
        return true;
      }
    }
  }else {
    if(fromX==toX) {
      if(fromY==toY-1 && chessBoard->getPiece(toX, toY) == EMPTY) {
        return true;
      }else if(fromY==toY-2 && fromY==1 && chessBoard->getPiece(toX, toY) == EMPTY && chessBoard->getPiece(toX, toY-1) == EMPTY) {
        return true;
      }
    }else if(fromX==toX-1 || fromX==toX+1) {
      if(fromY==toY-1 && ChessPiece::isBlack(chessBoard->getPiece(toX, toY))) {
        return true;
      }
    }
  }
  return false;
}

bool ChessRules::isValidMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY) {
  if(ChessPiece::isPawn(piece)) {
    return ChessRules::isValidPawnMove(chessBoard, piece, fromX, fromY, toX, toY);
  }else if(ChessPiece::isRook(piece)) {
    return ChessRules::isValidRookMove(chessBoard, piece, fromX, fromY, toX, toY);
  }else if(ChessPiece::isKnight(piece)) {
    return ChessRules::isValidKnightMove(chessBoard, piece, fromX, fromY, toX, toY);
  }else if(ChessPiece::isBishop(piece)) {
    return ChessRules::isValidBishopMove(chessBoard, piece, fromX, fromY, toX, toY);
  }else if(ChessPiece::isQueen(piece)) {
    return ChessRules::isValidQueenMove(chessBoard, piece, fromX, fromY, toX, toY);
  }else if(ChessPiece::isKing(piece)) {
    return ChessRules::isValidKingMove(chessBoard, piece, fromX, fromY, toX, toY);
  }
  return false;
}
