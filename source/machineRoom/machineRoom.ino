
#include <Stepper.h>
#include "Protocol.hpp"

const int stepsPerRevolution = 514;
int curFloor = 0;
int toFloor = 0;
int actionState = 0;
int adress = 1;

Protocol masterProtocol(1);
// initialize the stepper library on pins 8 through 11:
Stepper motor(stepsPerRevolution, 8, 9, 10, 11);

int buttonstate1 = 0;
int buttonstate0 = 0;

void setup() {
  Serial.begin(9600);
  motor.setSpeed(60);
}

void loop() 
{
    //sets the transmission address 7-seg display adress
    masterProtocol.setTransMissionAdress(3);
    masterProtocol.masterToSlavesCurFloor(curFloor);

    masterProtocol.makeProtolSlaveReader();
    
    
    if(adress == 5)
    {
      adress = 1;
    }
    masterProtocol.setTransMissionAdress(3);
    masterProtocol.makeProtolSlaveReader();

    actionState = masterProtocol.getAction();

    switch(actionState)
    {
      case 1:
        toFloor = masterProtocol.getFloor();
      break;

      case 2:
        curFloor = masterProtocol.getFloor();
      break;
    }

    if(toFloor != curFloor)
    {
      int steps = curFloor - toFloor;
      if(steps < 0)
      {
        motor.step(-stepsPerRevolution);
        motor.step(-stepsPerRevolution);
        motor.step(-stepsPerRevolution);
      }
      
      if(steps > 0)
      {
        motor.step(stepsPerRevolution);
        motor.step(stepsPerRevolution);
        motor.step(stepsPerRevolution);
      }
    }
    adress++;
}
