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

Protocol::Protocol(int tma)
{ 
  Wire.begin();
  transMissionAdress = tma;
}

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

//master protocol constructors
//============================================================================
void Protocol::makeProtolSlaveReader()
{
  Wire.requestFrom(transMissionAdress, 10);
  int i = 0;
  while(Wire.available())
  {
    char c = Wire.read();
    if(i == 0)
    {
      setAction(c);
    }
    if(i == 2)
    {
      setFloor(c);
    } 
  }
  
}

//slave protocol constructors 
//==============================================================================
//called by slave on elevator call
void Protocol::makeProtocolForCall()
{
  mAction = 1;
  Protocol::setSlaveReqeustMessage('r',1);
  Protocol::setSlaveReqeustMessage(mAction,0);
  Protocol::setSlaveReqeustMessage(20,0);
  Protocol::setSlaveReqeustMessage(mFloor,0);
  
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}

//called by slave when sensor detects
void Protocol::makeProtocolForDetection()
{
  mAction = 2;
  Protocol::setSlaveReqeustMessage('r',1);//cleans the static message
  Protocol::setSlaveReqeustMessage(mAction,0);
  Protocol::setSlaveReqeustMessage(20,0);
  Protocol::setSlaveReqeustMessage(mFloor,0);
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}
//sends curFloor to all the slaves
void Protocol::masterToSlavesCurFloor(int curFloor)
{
  Wire.beginTransmission(transMissionAdress);
  Wire.write(curFloor);
  Wire.endTransmission();
}

void Protocol::setSlaveReqeustMessage(int msg, int rest)
{
  if(rest)
  {
    slaveReqeustMessage = "";
  }else
  {
    if(msg == 20)//the code for the separator, :^))
    {
      slaveReqeustMessage += ",";
    }else
    {
      slaveReqeustMessage += msg;
    }
    
  }
}
char Protocol::slaveReqeustMessageCharArray[255];
String Protocol::slaveReqeustMessage;

String Protocol::getSlaveReqeustMessage()
{
  return slaveReqeustMessage;
}

void Protocol::slaveRequest()
{
  slaveReqeustMessage.toCharArray(slaveReqeustMessageCharArray, slaveReqeustMessage.length() +1);
  Wire.write(slaveReqeustMessageCharArray);
}
