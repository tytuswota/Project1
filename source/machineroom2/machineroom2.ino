#include <Stepper.h>
#include <Wire.h>

#define CHAR_RECEIVE_AMOUNT 3

char buffer[CHAR_RECEIVE_AMOUNT];

const int stepsPerRevolution = 64;
int queue[] = {0};
int curFloor = 0;
int destFloor = 0;
int mAction;
int mFloor;
int req = 0;
int transmissionAddress = 0;


Stepper motor(stepsPerRevolution, 8, 9, 10, 11);

int buttonstate1 = 0;
int buttonstate0 = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  motor.setSpeed(15);
}

void loop() {
    Wire.beginTransmission(transmissionAddress);
    Wire.write(curFloor);
    Wire.endTransmission();

    Wire.requestFrom(transmissionAddress, CHAR_RECEIVE_AMOUNT);
    memset(buffer, 0, CHAR_RECEIVE_AMOUNT);
    int i = 0;
    while(Wire.available()) {
      char c = Wire.read();
      buffer[i] = c;
      i++;
    }
    if(buffer[0]) {
      mAction = buffer[0]-'0';
      mFloor = buffer[2]-'0';
    }
    
    switch(mAction) {
      case 1:
        Serial.print("Request from floor ");
        destFloor = mFloor;
        mAction = 0;
        req = 1;
        break;
      case 2:
        Serial.print("Detection on floor ");
        curFloor = mFloor;
        mAction = 0;
        break;
    }
    if(mFloor) {
      Serial.println(mFloor);
      mFloor = 0;
    }

    if(req) {
      if(destFloor != curFloor) {
        Serial.print("going from floor ");
        Serial.print(curFloor);
        Serial.print(" to ");
        Serial.println(destFloor);
      } else {
        req = 0;
      }
    }
    
    transmissionAddress == 5 ? transmissionAddress = 0 : transmissionAddress++;
}
