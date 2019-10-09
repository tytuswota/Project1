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
  Wire.begin();
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
  slaveReqeustMessage = mAction + "," + mFloor;
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}

//called by slave when sensor detects
void Protocol::makeProtocolForDetection()
{
  mAction = 2;
  slaveReqeustMessage = mAction + "," + mFloor;
  Wire.begin(transMissionAdress);
  Wire.onRequest(Protocol::slaveRequest);
}

void Protocol::slaveRequest()
{
  Wire.write(slaveReqeustMessage);
}
