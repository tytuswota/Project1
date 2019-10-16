/*
protocol library for comonication between master/slave 
autor: Tymek Pisko
*/

#include <Arduino.h>
#include <Wire.h>

class MasterProtocol
{
  int transMissionAdress;
  int mAction,mFloor;

  static const int characterReceiveAmount = 3;
  char receiveBuffer[characterReceiveAmount+1];
  
  public:
    MasterProtocol(int tma);

    int getTransMissionAdress();
    int getAction();
    int getFloor();
    void setTransMissionAdress(int trans);
    void setAction(int act);
    void setFloor(int flr);
    
    //master functions
    //=================================
    void sendCurFloorToSlaves(int curFloor);
    void readSlave();
};
