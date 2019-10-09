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
//slave protocol constructors 
//==============================================================================
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
  //Wire.write("test");
}
