/*
protocol library for comonication between master/slave 
autor: Tymek Pisko
*/


/*
protocol: action, floor
actions
=======
1: request elevator
2: elevator detected
*/
#include "Protocol.hpp"
#include "slaveProtocol.hpp"

//constructor
//=======================================
Protocol::Protocol(int tma)
{ 
  Wire.begin(tma);
  transMissionAdress = tma;
  Wire.onReceive(Protocol::readFloor);
}

//geters and setters
//======================================
int Protocol::getAction()
{
  return mAction;
}

void Protocol::setTransMissionAdress(int trans)
{
  transMissionAdress = trans;
}

void Protocol::setAction(int act)
{
   mAction = act;
}

int Protocol::getFloor()
{
  return mFloor;
}

void Protocol::setFloor(int flr)
{
  mFloor = flr;
}

int Protocol::getTransMissionAdress()
{
  return transMissionAdress;
}

//master functions
//============================================================================
void Protocol::makeProtolSlaveReader()
{
  Wire.requestFrom(transMissionAdress, 4);
  int i = 0;
  while(Wire.available())
  {
    char c = Wire.read();
    if(i == 0)
    {
      Serial.println(c);
      setAction(c);
    }
    if(i == 2)
    {
      Serial.println(c);
      setFloor(c);
    }
    i++; 
  } 
}

//sends curFloor to all the slaves
void Protocol::snedCurFloorToSlaves(int curFloor)
{
  Wire.beginTransmission(transMissionAdress);
  Wire.write(curFloor);
  Wire.endTransmission();
}

//slave functions
//==============================================================================
//called by slave on elevator call
void Protocol::sendCallSignal(int action)
{
  mAction = action;
  Protocol::setSlaveReqeustMessage('r',1);
  Protocol::setSlaveReqeustMessage(mAction,0);
  Protocol::setSlaveReqeustMessage(20,0);
  Protocol::setSlaveReqeustMessage(mFloor,0);

  Serial.println(slaveReqeustMessage);
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}

//called by slave when sensor detects
void Protocol::sendDetectionSignal()
{
  mAction = 2;
  Protocol::setSlaveReqeustMessage('r',1);//cleans the static message
  Protocol::setSlaveReqeustMessage(mAction,0);
  Protocol::setSlaveReqeustMessage(20,0);
  Protocol::setSlaveReqeustMessage(mFloor,0);
  
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}

void Protocol::setCurFloor(int f)
{
  curFloor = f;
}

void Protocol::setSlaveReqeustMessage(int msg, int rest)
{
  if(rest)
  {
    slaveReqeustMessage = "";
  }else
  {
    if(msg == 20)
    {
      slaveReqeustMessage += ",";
    }else
    {
      slaveReqeustMessage += msg;
    }
    
  }
}

//event handler components
//============================================================
char Protocol::slaveReqeustMessageCharArray[4];
String Protocol::slaveReqeustMessage;
int Protocol::curFloor;
String Protocol::getSlaveReqeustMessage()
{
  return slaveReqeustMessage;
}
//slave request event handler
void Protocol::slaveRequest()
{
  slaveReqeustMessage.toCharArray(slaveReqeustMessageCharArray, slaveReqeustMessage.length() +1);
  Wire.write(slaveReqeustMessageCharArray);
  Wire.onRequest(NULL);
}

int Protocol::getCurFloor()
{
  return curFloor;  
}

void Protocol::readFloor(int len)
{
  /*while(1 < Wire.available())
  {
    int c = Wire.read();
    Serial.print(c);
  }*/
  int x = Wire.read();
  curFloor = x;
  Serial.println(x);
}
