#ifndef CHESSPIECESELECTOR_H
#define CHESSPIECESELECTOR_H

class ChessPieceSelectorListener {
  public:
    virtual void chessPieceSelectorPieceSelected(char piece) = 0;
};

class ChessPieceSelector {
  private:
    int defaultValue[6];
    int pins[6];
    int sensitivity;
    ChessPieceSelectorListener *listener;
  public:
    ChessPieceSelector(int sensitivity, int pawnPin, int rookPin, int knightPin, int bishopPin, int queenPin, int kingPin);
    void setListener(ChessPieceSelectorListener* listener);
    void init();
    void refresh();
    void scan(int defaultValue, int pin, char whitePiece, char blackPiece);
};

#endif
