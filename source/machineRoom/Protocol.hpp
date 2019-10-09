/*
protocol library for comonication between master/slave 
autor: Tymek Pisko
*/

#include <Arduino.h>
#include <Wire.h>

class Protocol
{
  static char* slaveReqeustMessage;
  String message;
  int transMissionAdress;
  int mAction,mFloor;
  
  public:
  Protocol(int tma);

  int getTransMissionAdress();
  int getAction();
  int getFloor();
  void setTransMissionAdress(int trans);
  void setAction(int act);
  void setFloor(int flr);
  void makeProtocolForDetection();
  void makeProtocolForCall();
  
  void makeProtolSlaveResever();
  static void slaveRequest();
};
