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
#include "MasterProtocol.hpp"

//constructor
//=======================================
MasterProtocol::MasterProtocol(int transmissionAddress)
{ 
  Wire.begin();
  this->transMissionAdress = transmissionAddress;
}

//geters and setters
//======================================
int MasterProtocol::getAction()
{
  return mAction;
}
void MasterProtocol::setTransMissionAdress(int transmissionAddress)
{
  this->transMissionAdress = transmissionAddress;
}

void MasterProtocol::setAction(int act)
{
   this->mAction = act;
}

int MasterProtocol::getFloor()
{
  return mFloor;
}

void MasterProtocol::setFloor(int flr)
{
  this->mFloor = flr;
}

int MasterProtocol::getTransMissionAdress()
{
  return transMissionAdress;
}

//master functions
//============================================================================
void MasterProtocol::readSlave()
{
  Wire.requestFrom(transMissionAdress, characterReceiveAmount);
  //Serial.println(transMissionAdress);
  int i = 0;
  memset(receiveBuffer, 0, characterReceiveAmount);
  while(Wire.available())
  {
    char c = Wire.read();
    receiveBuffer[i] = c;
    i++; 
  }
  setAction(receiveBuffer[0]);
  setFloor(receiveBuffer[2]);
  
  if(receiveBuffer[0]) {
    Serial.println(receiveBuffer);
  }
}

//sends curFloor to all the slaves
void MasterProtocol::sendCurFloorToSlaves(int curFloor)
{
  Wire.beginTransmission(transMissionAdress);
  Wire.write(curFloor);
  Wire.endTransmission();
}
