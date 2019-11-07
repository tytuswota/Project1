
#include "ShiftOutRegister.hpp"
#include "Protocol.hpp"

#define REQUP 7
#define REQDOWN 6
#define INFRED 5
#define CLOSEDLED 3
#define OPENLED 2

const byte lookup[10] = {
  B01111110,
  B00011000,
  B10110110,
  B10111100,
  B11011000,
  B11100110,
  B11101110,
  B00110010,
  B11111110,
  B11110110
};

ShiftOutRegister shiftreg(8, 9, 10);
int rDown = 0;
int rUp = 0;
int detect = 0;
int state = 0;

//makes protocol object sets the transmission adress to 3
//=======================================================
Protocol slaveProtocol(4);

void setup() 
{
  pinMode(REQUP, INPUT_PULLUP);
  pinMode(REQDOWN, INPUT_PULLUP);
  pinMode(INFRED, INPUT_PULLUP);
  pinMode(CLOSEDLED, OUTPUT);
  pinMode(OPENLED, OUTPUT);
  Serial.begin(9600);
  slaveProtocol.setFloor(4);
}

int active = 0;
int detectionActive = 0;

void loop() 
{ 
  rDown = 1 - digitalRead(REQDOWN);
  rUp = 1 - digitalRead(REQUP);
  detect = 1 - digitalRead(INFRED);
  slaveProtocol.setTransMissionAdress(4);
  slaveProtocol.setFloor(4);
  if(rDown)
  {
    state = 1;
  }
  else if(rUp)
  {
    state = 2;
  }
  else if(detect)
  {
    state = 3; 
  }

  switch(state)
  {
    case 1:
      if(!active)
      {
        slaveProtocol.sendCallSignal(1);
        active = 1;
        detectionActive = 0;
        digitalWrite(CLOSEDLED, HIGH);
      }
    break;

    case 2:
      if(!active)
      {
        slaveProtocol.sendCallSignal(3);
        active = 1;
        detectionActive = 0;
        digitalWrite(CLOSEDLED, HIGH);
      }
    break;
    
    case 3:
       if(!detectionActive)
       {
        shiftreg.set(lookup[4]);
        shiftreg.show();
        slaveProtocol.sendDetectionSignal();
        detectionActive = 1;
       
       }else if(active)
       {
        digitalWrite(CLOSEDLED, LOW);
        digitalWrite(OPENLED, HIGH);
        active = 0;
       } 
       else
       {
        state = 0;
       }
       
    break;
    
    default:
      //Serial.println("in defualt");
      active = 0;
      detectionActive = 0;
      digitalWrite(CLOSEDLED, LOW);
      digitalWrite(OPENLED, LOW);
      shiftreg.set(lookup[slaveProtocol.getCurFloor()]);
      //shiftreg.set(lookup[4]);
      shiftreg.show();
    break;
  }
}
