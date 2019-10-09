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
  
  int getAction();
  int getFloor();
  void setAction(int act);
  void setFloor(int flr);
  void makeProtocolForDetection();
  void makeProtocolForCall();
  static void slaveRequest();
};
