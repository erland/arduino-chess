#include <Arduino.h>
#include "chessboard.h"

ChessBoard::ChessBoard(SensorMatrix *sensorMatrix, Adafruit_8x16matrix *ledMatrix) {
  this->sensorMatrix = sensorMatrix;
  this->sensorMatrix->setListener(this);
  this->ledMatrix = ledMatrix;
  for(int y=0;y<8;y++) {
    for(int x=0;x<8;x++) {
      char sensorState = sensorMatrix->getSensorState(x, y);
      if(sensorState == SENSOR_STATE_EMPTY) {
        pieces[y*8+x] = EMPTY;
      }else if(sensorState == SENSOR_STATE_WHITE) {
        pieces[y*8+x] = WHITE_UNKNOWN;
      }else {
        pieces[y*8+x] = BLACK_UNKNOWN;
      }
    }
  }
  currentlyMoved = EMPTY;
  lastChanged = 0;
}

bool ChessBoard::isWhite(char piece) {
  if(piece == WHITE_PAWN || piece == WHITE_ROOK || piece == WHITE_KNIGHT || piece == WHITE_BISHOP || piece == WHITE_QUEEN || piece == WHITE_KING || piece == WHITE_UNKNOWN) {
    return true;
  }
  return false;
}

void ChessBoard::setCurrentlyMoved(char piece) {
  if(currentlyMoved != piece) {
    currentlyMoved = piece;
    lastChanged = millis();
    refreshLedMatrix();
  }
}

void ChessBoard::sensorMatrixChanged(int x, int y, char oldValue, char newValue) {
  if(pieces[y*8+x] != EMPTY) {
    if(newValue == SENSOR_STATE_EMPTY) {
      if(currentlyMoved == EMPTY) {
        Serial.println("Piece lifted");
        // Piece lifted
        currentlyMoved = pieces[y*8+x];
      }else {
        if((oldValue==SENSOR_STATE_WHITE && isWhite(currentlyMoved)) || (oldValue==SENSOR_STATE_BLACK && !isWhite(currentlyMoved))) {
          Serial.println("Another piece lifted, switching currently moved");
          currentlyMoved = pieces[y*8+x];
        }else {
          Serial.println("Piece of other coller lifted, this is a strike");
          // Ignore, this is likely a strike
        }
      }
      pieces[y*8+x] = EMPTY;
    }else {
      Serial.print("This is just strange");
    }
  }else {
    if(oldValue == SENSOR_STATE_EMPTY && newValue != SENSOR_STATE_EMPTY) {
      // Piece placed
      if(currentlyMoved != EMPTY && (newValue == SENSOR_STATE_WHITE)==isWhite(currentlyMoved)) {
        Serial.print("Placed: ");Serial.println(currentlyMoved);
        pieces[y*8+x] = currentlyMoved;
        currentlyMoved = EMPTY;
      }else if(newValue == SENSOR_STATE_WHITE) {
        pieces[y*8+x] = WHITE_UNKNOWN;
        Serial.print("Placed white unknown");
      }else {
        pieces[y*8+x] = BLACK_UNKNOWN;
        Serial.print("Placed black unknown");
      }
    }else {
        Serial.println("Just strange, this should not happen");
    }
  }
  lastChanged = millis();
  refreshLedMatrix();
}

char ChessBoard::getPiece(int x, int y) {
  return pieces[y*8+x];
}

void ChessBoard::refreshLedMatrix() {
  ledMatrix->clear(); 
  Serial.println("+--------+");
  for ( int y=0;y<8;y++) {
    Serial.print("|");
    for (int x=0;x<8;x++) {
      char m = getPiece(x, y);
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
  if(currentlyMoved == WHITE_PAWN || currentlyMoved == BLACK_PAWN) {
    activateCurrentlyPlacedLed(0);
  }else if(currentlyMoved == WHITE_ROOK || currentlyMoved == BLACK_ROOK) {
    activateCurrentlyPlacedLed(1);
  }else if(currentlyMoved == WHITE_KNIGHT || currentlyMoved == BLACK_KNIGHT) {
    activateCurrentlyPlacedLed(2);
  }else if(currentlyMoved == WHITE_BISHOP || currentlyMoved == BLACK_BISHOP) {
    activateCurrentlyPlacedLed(3);
  }else if(currentlyMoved == WHITE_QUEEN || currentlyMoved == BLACK_QUEEN) {
    activateCurrentlyPlacedLed(4);
  }else if(currentlyMoved == WHITE_KING || currentlyMoved == BLACK_KING) {
    activateCurrentlyPlacedLed(5);
  }
  Serial.println();
  ledMatrix->writeDisplay();
}

void ChessBoard::activateCurrentlyPlacedLed(int index) {
  if(index<3) {
    enableLed(8, index);
  }else {
    enableLed(8, 2+index);
  }
}

long ChessBoard::getLastChanged() {
  return lastChanged;
}
void ChessBoard::enableLed(int x, int y) {
  ledMatrix->drawPixel(7-y, x, 1);  
}

const char* ChessBoard::getBoardArray() {
  return pieces;
}
