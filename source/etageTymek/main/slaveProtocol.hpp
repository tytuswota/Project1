/*
protocol library for comonication between master/slave 
autor: Tymek Pisko
*/

#include <Arduino.h>
#include <Wire.h>

class SlaveProtocol
{
  static String slaveReqeustMessage;
  int transMissionAdress;
  int mAction,mFloor;
  
  public:
    SlaveProtocol(int tma);

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
   
    //static functions for eventhandler
    //=================================
    static void slaveRequest();
    static void setSlaveReqeustMessage(int msg, int rest);
    static String getSlaveReqeustMessage();
    static char slaveReqeustMessageCharArray[];
    
    
    
};
