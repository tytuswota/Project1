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
  static int curFloor;
  
  public:
    Protocol(int tma);

    int getTransMissionAdress();
    int getAction();
    static int getCurFloor();
    static void setCurFloor(int f);
    int getFloor();
    //void setCurFloor(int f);
    void setTransMissionAdress(int trans);
    void setAction(int act);
    void setFloor(int flr);

    //slave functions
    //=================================
    void sendDetectionSignal();
    void sendCallSignal(int action);
    
    //master functions
    //=================================
    void snedCurFloorToSlaves(int curFloor);
    void makeProtolSlaveReader();
    
    //static functions for eventhandler
    //=================================
    static void slaveRequest();
    static void readFloor(int len);
    static void setSlaveReqeustMessage(int msg, int rest);
    static String getSlaveReqeustMessage();
    static char slaveReqeustMessageCharArray[];
    
};
