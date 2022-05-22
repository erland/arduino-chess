#include "chesscontroller.h"
#include "chessrules.h"
#include "chesspiece.h"

ChessController::ChessController(RotaryController* rotaryController, SensorMatrix* sensorMatrix, ChessPieceSelector* chessPieceSelector, ChessBoard* chessBoard, ChessDisplay* chessDisplay, Adafruit_8x16matrix *ledMatrix) {
  this->rotaryController = rotaryController;
  this->rotaryController->setListener(this);
  this->chessPieceSelector = chessPieceSelector;
  this->chessPieceSelector->setListener(this);
  this->sensorMatrix = sensorMatrix;
  this->sensorMatrix->setListener(this);
  this->chessBoard = chessBoard;
  this->chessDisplay = chessDisplay;
  this->ledMatrix = ledMatrix;
  currentlyMoved = EMPTY;
  currentlyMovedPos = -1;
  strikePos = -1;
}

void ChessController::init() {
  for(int y=0;y<8;y++) {
    for(int x=0;x<8;x++) {
      char sensorState = sensorMatrix->getSensorState(x, y);
      if(sensorState == SENSOR_STATE_EMPTY) {
        chessBoard->setPiece(x, y, EMPTY);
      }else if(sensorState == SENSOR_STATE_WHITE) {
        chessBoard->setPiece(x, y, WHITE_UNKNOWN);
      }else {
        chessBoard->setPiece(x, y, BLACK_UNKNOWN);
      }
    }
  }

}
void ChessController::rotaryControllerChangedRotation(int value, int offset) {
  
}
void ChessController::rotaryControllerButtonDown() {
  // Do nothing
}
void ChessController::rotaryControllerButtonUp() {
  // Do nothing
}

void ChessController::sensorMatrixChanged(int x, int y, char oldValue, char newValue) {
  char previousPiece = chessBoard->getPiece(x, y);
  if(previousPiece != EMPTY) {
    if(newValue == SENSOR_STATE_EMPTY) {
      if(currentlyMoved == EMPTY) {
        Serial.println("Piece lifted");
        // Piece lifted
        currentlyMoved = previousPiece;
        currentlyMovedPos = y*8+x;
        strikePos=-1;
      }else {
        if((oldValue==SENSOR_STATE_WHITE && ChessPiece::isWhite(currentlyMoved)) || (oldValue==SENSOR_STATE_BLACK && !ChessPiece::isWhite(currentlyMoved))) {
          Serial.println("Another piece lifted, switching currently moved");
          currentlyMoved = previousPiece;
          currentlyMovedPos = y*8+x;
          strikePos=-1;
        }else {
          Serial.println("Piece of other color lifted, this is a strike");
          // Ignore, this is likely a strike
          strikePos=y*8+x;
        }
      }
      chessBoard->setPiece(x, y, EMPTY);
    }else {
      Serial.print("This is just strange");
    }
  }else {
    if(oldValue == SENSOR_STATE_EMPTY && newValue != SENSOR_STATE_EMPTY) {
      // Piece placed
      if(currentlyMoved != EMPTY && (newValue == SENSOR_STATE_WHITE)==ChessPiece::isWhite(currentlyMoved)) {
        Serial.print("Placed: ");Serial.println(currentlyMoved);
        chessBoard->setPiece(x, y, currentlyMoved);
        currentlyMoved = EMPTY;
        currentlyMovedPos = -1;
        strikePos=-1;
      }else if(newValue == SENSOR_STATE_WHITE) {
        chessBoard->setPiece(x, y, WHITE_UNKNOWN);
        Serial.print("Placed white unknown");
      }else {
        chessBoard->setPiece(x, y, BLACK_UNKNOWN);
        Serial.print("Placed black unknown");
      }
    }else {
        Serial.println("Just strange, this should not happen");
    }
  }
  refreshLedMatrix();
}

void ChessController::enableLed(int x, int y) {
  ledMatrix->drawPixel(7-y, x, 1);  
}

void ChessController::refreshLedMatrix() {
  ledMatrix->clear(); 
  if(currentlyMovedPos<0 && strikePos<0) {
    Serial.println("+--------+");
    for ( int y=0;y<8;y++) {
      Serial.print("|");
      for (int x=0;x<8;x++) {
        char m = chessBoard->getPiece(x, y);
        if(m != EMPTY) {
          enableLed(x,y);
          Serial.print(m);
        }else {
          Serial.print("_");
        }
      }
      Serial.println("|");
    }
    Serial.println("+--------+");
    Serial.println();
    Serial.println(currentlyMoved);
  }else if(strikePos>=0) {
    enableLed(strikePos%8,strikePos/8);
  }else {
    for(int y=0;y<8;y++) {
      for(int x=0;x<8;x++) {
        if(y != currentlyMovedPos/8 || x!= currentlyMovedPos%8) {
          if(ChessRules::isValidMove(chessBoard, currentlyMoved, currentlyMovedPos%8, currentlyMovedPos/8, x, y)) {
            enableLed(x, y);
          }
        }
      }
    }
  }
  if(ChessPiece::isPawn(currentlyMoved)) {
    activateCurrentlyPlacedLed(0);
  }else if(ChessPiece::isRook(currentlyMoved)) {
    activateCurrentlyPlacedLed(1);
  }else if(ChessPiece::isKnight(currentlyMoved)) {
    activateCurrentlyPlacedLed(2);
  }else if(ChessPiece::isBishop(currentlyMoved)) {
    activateCurrentlyPlacedLed(3);
  }else if(ChessPiece::isQueen(currentlyMoved)) {
    activateCurrentlyPlacedLed(4);
  }else if(ChessPiece::isKing(currentlyMoved)) {
    activateCurrentlyPlacedLed(5);
  }
  Serial.println();
  ledMatrix->writeDisplay();
}

void ChessController::activateCurrentlyPlacedLed(int index) {
  if(index<3) {
    enableLed(8, index);
  }else {
    enableLed(8, 2+index);
  }
}

void ChessController::chessPieceSelectorPieceSelected(char piece) {
  currentlyMoved = piece;
  currentlyMovedPos = -1;
  strikePos=-1;
  refreshLedMatrix();
}
