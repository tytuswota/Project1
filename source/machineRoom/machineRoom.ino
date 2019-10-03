#include <Stepper.h>
#include <Wire.h>

const int stepsPerRevolution = 514;
const int cur_floor = 0;

//"action: "", floor: """
//actions:
//elivator request
//elivator detection

// initialize the stepper library on pins 8 through 11:
Stepper motor(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  motor.setSpeed(60);
  //wire.begin();
}

void loop() {
    //int steps = 360;
    motor.step(stepsPerRevolution);
    motor.step(stepsPerRevolution);
    motor.step(stepsPerRevolution);
    motor.step(stepsPerRevolution);
    motor.step(stepsPerRevolution);
    delay(100);

    //steps = -360;
    motor.step(-stepsPerRevolution);
    delay(500);
  
}
