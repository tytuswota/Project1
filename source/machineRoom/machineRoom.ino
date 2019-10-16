//#include <MasterProtocol.h>
#include <Stepper.h>
#include "MasterProtocol.hpp"

const int stepsPerRevolution = 514;
int queue[] = {0};
int curFloor = 0;
int toFloor = 0;
int actionState = 0;
int adress = 1;

MasterProtocol masterProtocol(1);
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
    masterProtocol.setTransMissionAdress(adress);
    masterProtocol.sendCurFloorToSlaves(curFloor);
    
    masterProtocol.readSlave();
    
    if(adress == 5)
    {
      adress = 1;
    }

    actionState = masterProtocol.getAction();
    
    switch(actionState)
    {
      case 1:
        toFloor = masterProtocol.getFloor();
        queue[adress] = toFloor;
        actionState = 0;
      break;

      case 2:
        curFloor = masterProtocol.getFloor();
        actionState = 0;
      break;
    }

    if(toFloor != curFloor)
    {
      int steps = curFloor - toFloor;
      Serial.println(steps);
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
