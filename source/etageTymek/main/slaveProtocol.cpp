#include "slaveProtocol.hpp"
//geters and setters
//======================================
int SlaveProtocol::getAction()
{
  return mAction;
}
void SlaveProtocol::setTransMissionAdress(int transmissionAddress)
{
  this->transMissionAdress = transmissionAddress;
}

void SlaveProtocol::setAction(int act)
{
   this->mAction = act;
}

int SlaveProtocol::getFloor()
{
  return mFloor;
}

void SlaveProtocol::setFloor(int flr)
{
  this->mFloor = flr;
}

int SlaveProtocol::getTransMissionAdress()
{
  return transMissionAdress;
}
//slave functions
//==============================================================================
//called by slave on elevator call

void SlaveProtocol::sendCallSignal()
{
  mAction = 1;
  SlaveProtocol::setSlaveReqeustMessage('r',1);
  SlaveProtocol::setSlaveReqeustMessage(mAction,0);
  SlaveProtocol::setSlaveReqeustMessage(20,0);
  SlaveProtocol::setSlaveReqeustMessage(mFloor,0);
  
  Wire.begin(transMissionAdress);
  Wire.onRequest(SlaveProtocol::slaveRequest);
}

//called by slave when sensor detects
void SlaveProtocol::sendDetectionSignal()
{
  mAction = 2;
  SlaveProtocol::setSlaveReqeustMessage('r',1);//cleans the static message
  SlaveProtocol::setSlaveReqeustMessage(mAction,0);
  SlaveProtocol::setSlaveReqeustMessage(20,0);
  SlaveProtocol::setSlaveReqeustMessage(mFloor,0);
  
  Wire.begin(transMissionAdress);
  Wire.onRequest(SlaveProtocol::slaveRequest);
}

void SlaveProtocol::setSlaveReqeustMessage(int msg, int rest)
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
char SlaveProtocol::slaveReqeustMessageCharArray[4];
String SlaveProtocol::slaveReqeustMessage;

String SlaveProtocol::getSlaveReqeustMessage()
{
  return slaveReqeustMessage;
}
//slave request event handler

void SlaveProtocol::slaveRequest()
{
  slaveReqeustMessage.toCharArray(slaveReqeustMessageCharArray, slaveReqeustMessage.length() +1);
  Wire.write(slaveReqeustMessageCharArray);
  slaveReqeustMessage = "0";
}
