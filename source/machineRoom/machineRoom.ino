
#include <Stepper.h>
#include <Wire.h>
#include "ShiftOutRegister.hpp"
//#include "MessageDecoder.h"
const int button_Up = 4;
const int button_Down = 5;
const int stepsPerRevolution = 514;
const int cur_floor = 0;
const int ProxSensor_1 = 3;
const int ProxSensor_0 = 7;
int state = 0;
//"action: "", floor: """
//actions:
//elivator request
//elivator detection

// initialize the stepper library on pins 8 through 11:
Stepper motor(stepsPerRevolution, 8, 9, 10, 11);

int buttonstate1 = 0;
int buttonstate0 = 0;

void setup() {
  state = 0;
  pinMode (button_Up = INPUT);
  pinMode (button_Down = INPUT);
  pinMode(3, OUTPUT);   // setting the pin modes, the "13" stands for the internal Arduino uno internal LED
  pinMode(ProxSensor_1, INPUT);
  pinMode(ProxSensor_0, INPUT);
  Serial.begin(9600);
  motor.setSpeed(60);
  //wire.begin();
}

void loop() {
  Serial.println();
  buttonState1 = digitalRead(button_Up);
  buttonState0 = digitalRead(button_Down);
  switch (state) 
  {
    case 0:
        // Decimal 63      Binary output  01111111     Displays 0
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 63);
        digitalWrite(latchPin, HIGH);
        delay(1000);
        //should check if you want to go up
        if (buttonState1 == HIGH ) {
          motor.setSpeed(60);
          motor.step(stepsPerRevolution);
        }

        if (digitalRead(ProxSensor_1) == LOW)   //Check the sensor output if it's high
        {
          motor.setSpeed(0);
          state = 1;
        }
      case 1:
        // Decimal 6      Binary output  00000110     Displays 1
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 6);
        digitalWrite(latchPin, HIGH);
        delay(1000);

        if (buttonState0 = HIGH)
        {
          motor.setSpeed(60);
          motor.step(-stepsPerRevolution);
        }
        
        if (digitalRead(ProxSensor_0) == LOW)   //Check the sensor output if it's high
        {
          motor.setSpeed(0);
          state = 0;
        }
  }
}
