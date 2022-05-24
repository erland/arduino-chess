#ifndef CHESSRULES_H
#define CHESSRULES_H

class ChessBoardInterface {
  public:
    virtual char getPiece(int x, int y) = 0;
};

class ChessRules {
  private:
    static bool isValidKingMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    static bool isValidQueenMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    static bool isValidBishopMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    static bool isValidKnightMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    static bool isValidRookMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    static bool isValidPawnMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    static bool isCheck(ChessBoardInterface* chessBoard, char king, int kingX, int kingY);
  public:
    static bool isValidMove(ChessBoardInterface* chessBoard, char piece, int fromX, int fromY, int toX, int toY);
    
};

#endif
