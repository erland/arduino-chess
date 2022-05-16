#ifndef SENSORMATRIX_H
#define SENSORMATRIX_H

#define SENSOR_STATE_WHITE 'W'
#define SENSOR_STATE_BLACK 'B'
#define SENSOR_STATE_EMPTY ' '

class SensorMatrixListener {
  public:
    virtual void sensorMatrixChanged(int x, int y, char fromState, char toState) = 0;
};

class SensorMatrix {
  private:
    int (*muxSignalPins)[4];
    int (*muxControlPins)[4][4];
    int (*muxSquareMapping)[64];
    int defaultValue[64];
    char sensor_states[64];
    SensorMatrixListener* listener;
    int sensitivity;
    void setSensorState(int x, int y, char state);
    int readMux(int channel);
  public:
    SensorMatrix(int sensitivity, int (*muxControlPins)[4][4], int (*muxSignalPins)[4], int (*muxSquareMapping)[64]);
    void setListener(SensorMatrixListener* listener);
    void init();
    void refresh();
    char getSensorState(int x, int y);
    
};
#endif
