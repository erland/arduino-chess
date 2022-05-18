#ifndef CHESSPIECE_H
#define CHESSPIECE_H

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

class ChessPiece {
  public:
    static bool isWhite(char piece);
    static bool isKing(char piece);
    static bool isQueen(char piece);
    static bool isBishop(char piece);
    static bool isKnight(char piece);
    static bool isRook(char piece);
    static bool isPawn(char piece);
};

#endif
