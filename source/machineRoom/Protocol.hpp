/*
protocol library for comonication between master/slave 
autor: Tymek Pisko
*/

#include <Arduino.h>
#include <Wire.h>

class Protocol
{
  static String slaveReqeustMessage;
  int transMissionAdress;
  int mAction,mFloor;

  static const int master_characterReceiveAmount = 3;
  char receiveBuffer[master_characterReceiveAmount+1];
  
  public:
    Protocol(int tma);

    int getTransMissionAdress();
    int getAction();
    int getFloor();
    void setTransMissionAdress(int trans);
    void setAction(int act);
    void setFloor(int flr);

    //slave functions
    //=================================
    void sendDetectionSignal();
    void sendCallSignal();
    
    //master functions
    //=================================
    void sendCurFloorToSlaves(int curFloor);
    void makeProtolSlaveReader();
    
    //static functions for eventhandler
    //=================================
    static void slaveRequest();
    static void setSlaveReqeustMessage(int msg, int rest);
    static String getSlaveReqeustMessage();
    static char slaveReqeustMessageCharArray[];
    
};
