
#include "ShiftOutRegister.hpp"
#include "Protocol.hpp"

#define reqUp 7
#define reqDown 6
#define infRed 5
#define closedLed 3
#define openLed 2
const byte lookup[10] = {
  B01111110,
  B00010010,
  B10111100,
  B10110110,
  B11010010,
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

Protocol slaveProtocol(3);

void setup() {
  pinMode(reqUp, INPUT_PULLUP);
  pinMode(reqDown, INPUT_PULLUP);
  pinMode(infRed, INPUT_PULLUP);
  pinMode(closedLed, OUTPUT);
  pinMode(openLed, OUTPUT);
  slaveProtocol.setFloor(3);
  Serial.begin(9600);
}

int active = 0;
int detectionActive = 0;

void loop() 
{ 
  rDown = 1 - digitalRead(reqDown);
  rUp = 1 - digitalRead(reqUp);
  detect = 1 - digitalRead(infRed);
  
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
        slaveProtocol.makeProtocolForCall();
        active = 1;
        detectionActive = 0;
        digitalWrite(closedLed, HIGH);
      }
    break;

    case 2:
      if(!active)
      {
        slaveProtocol.makeProtocolForCall();
        active = 1;
        detectionActive = 0;
        digitalWrite(closedLed, HIGH);
      }
    break;
    
    case 3:
       if(!detectionActive)
       {
        shiftreg.set(lookup[3]);
        shiftreg.show();
        slaveProtocol.makeProtocolForDetection();
        detectionActive = 1;
       
       }else if(active)
       {
        digitalWrite(closedLed, LOW);
        digitalWrite(openLed, HIGH);
        active = 0;
       } 
       else
       {
        state = 0;
        
       }
       
    break;
    
    default:
      Serial.println("in defualt");
      active = 0;
      detectionActive = 0;
      digitalWrite(closedLed, LOW);
      digitalWrite(openLed, LOW);
      shiftreg.set(lookup[0]);
      shiftreg.show();
    break;
  }
}
