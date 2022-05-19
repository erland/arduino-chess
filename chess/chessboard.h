#ifndef CHESSBOARD_H
#define CHESSBOARD_H


class ChessBoard {
  private: 
    char pieces[64];
    long lastChanged;
  public:
    ChessBoard();
    char getPiece(int x, int y);
    void setPiece(int x, int y, char piece);
    void setStartPosition();
    bool isStartPosition();
    long getLastChanged();
    const char* getBoardArray();
    char* getBoardFENNotation();
};

#endif
