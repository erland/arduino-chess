#include <Arduino.h>
#include "chessrules.h"
#include "chesspiece.h"

struct Offset {
  int offsetX;
  int offsetY;
};

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

bool ChessRules::isCheck(ChessBoardInterface* chessBoard, char king, int kingX, int kingY) {
  bool isWhiteKing = ChessPiece::isWhite(king);

  Offset rowLineOffsets[4] = {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
  };

  for(int i=0;i<4;i++) {
    for(int offset=1;offset<7;offset++) {
      int x = kingX+rowLineOffsets[i].offsetX*offset;
      int y = kingY+rowLineOffsets[i].offsetY*offset;
      if(x>=0 && x<8 && y>=0 && y<8) {
        char piece = chessBoard->getPiece(x, y);
        if(piece == EMPTY) {
          continue;
        }else if(ChessPiece::isBlack(piece)==isWhiteKing && (ChessPiece::isRook(piece) || ChessPiece::isQueen(piece))) {
          return true;
        }else {
          break;
        }
      }
    }
  }
  
  Offset diagonalOffsets[4] = {
    {1, 1},
    {1, -1},
    {-1, 1},
    {-1, -1}
  };

  for(int i=0;i<4;i++) {
    for(int offset=1;offset<7;offset++) {
      int x = kingX+diagonalOffsets[i].offsetX*offset;
      int y = kingY+diagonalOffsets[i].offsetY*offset;
      if(x>=0 && x<8 && y>=0 && y<8) {
        char piece = chessBoard->getPiece(x, y);
        if(piece == EMPTY) {
          continue;
        }else if(ChessPiece::isBlack(piece)==isWhiteKing && (ChessPiece::isBishop(piece) || ChessPiece::isQueen(piece))) {
          return true;
        }else {
          break;
        }
      }
    }
  }


  Offset knightOffsets[8] = {
    {2, 1},
    {2, -1},
    {-2, 1},
    {-2, -1},
    {1, 2},
    {-1, 2},
    {1, -2},
    {-1, -2}
  };

  for(int i=0;i<8;i++) {
    int x = kingX+knightOffsets[i].offsetX;
    int y = kingY+knightOffsets[i].offsetY;
    if(x>=0 && x<8 && y>=0 && y<8) {
      char piece = chessBoard->getPiece(x, y);
      if(ChessPiece::isBlack(piece)==isWhiteKing && ChessPiece::isKnight(piece)) {
        return true;
      }
    }
  }

  if(isWhiteKing) {
    if(kingY<6) {
      if(kingX>0 && chessBoard->getPiece(kingX-1,kingY+1) == BLACK_PAWN) {
        return true;
      }
      if(kingX<7 && chessBoard->getPiece(kingX+1,kingY+1) == BLACK_PAWN) {
        return true;
      }
    }
  }else {
    if(kingY>1) {
      if(kingX>0 && chessBoard->getPiece(kingX-1,kingY-1) == WHITE_PAWN) {
        return true;
      }
      if(kingX<7 && chessBoard->getPiece(kingX+1,kingY-1) == WHITE_PAWN) {
        return true;
      }
    }
  }
  Offset kingOffsets[8] = {
    {-1, -1},
    {0, -1},
    {1, -1},
    {1, 0},
    {1, 1},
    {0, 1},
    {-1, 1},
    {-1, 0}
  };
  for(int i=0;i<8;i++) {
    int x = kingX+kingOffsets[i].offsetX;
    int y = kingY+kingOffsets[i].offsetY;
    if(x>=0 && x<8 && y>=0 && y<8) {
      char piece = chessBoard->getPiece(x, y);
      if(ChessPiece::isBlack(piece)==isWhiteKing && ChessPiece::isKing(piece)) {
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
    return ChessRules::isValidKingMove(chessBoard, piece, fromX, fromY, toX, toY) && !ChessRules::isCheck(chessBoard, piece, toX, toY);
  }
  return false;
}
