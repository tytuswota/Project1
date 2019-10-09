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
void Protocol::makeProtolSlaveResever()
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
  Protocol::setSlaveReqeustMessage(mAction,1);
  Protocol::setSlaveReqeustMessage('1',0);
  Protocol::setSlaveReqeustMessage(',',0);
  Protocol::setSlaveReqeustMessage(char(mFloor),0);
  
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}
void Protocol::setSlaveReqeustMessage(char msg, int rest)
{
  if(rest)
  {
    slaveReqeustMessage = "";
  }else
  {
    slaveReqeustMessage += msg;
  }
}
char Protocol::slaveReqeustMessageCharArray[255];
String Protocol::slaveReqeustMessage;

String Protocol::getSlaveReqeustMessage()
{
  return slaveReqeustMessage;
}


//called by slave when sensor detects
/*void Protocol::makeProtocolForDetection()
{
  mAction = 2;
  slaveReqeustMessage = mAction + "," + mFloor;
  //Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}*/

void Protocol::slaveRequest()
{
  slaveReqeustMessage.toCharArray(slaveReqeustMessageCharArray, slaveReqeustMessage.length() +1);
  Wire.write(slaveReqeustMessageCharArray);
}
