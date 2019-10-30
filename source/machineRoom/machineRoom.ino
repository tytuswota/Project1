#include <Wire.h>

#define CHAR_RECEIVE_AMOUNT 4

char buffer[CHAR_RECEIVE_AMOUNT];

class ControlledMotor {
  public:
  int pin1, pin2;
  ControlledMotor(int pin1, int pin2) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }
  void spinUp() {
    digitalWrite(pin1, 1);
    digitalWrite(pin2, 0);
  }
  void spinDown() {
    digitalWrite(pin1, 0);
    digitalWrite(pin2, 1);
  }
  void stop() {
    digitalWrite(pin1, 0);
    digitalWrite(pin2, 0);
  }
};

const int stepsPerRevolution = 64;
int curFloor = 0;
int destFloor = 0;
int mAction;
int mFloor;
int req = 0;
int mDestination;
int transmissionAddress = 0;
int liftDirection = 0;
int reqDirection = 0;


//Stepper motor(stepsPerRevolution, 8, 9, 10, 11);
ControlledMotor motor(5, 6);

int buttonstate1 = 0;
int buttonstate0 = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
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
        Serial.print("Request from up floor ");
        mAction = 0;
        destFloor = mFloor;
        reqDirection = 1;
        req = 1;
        break;
      case 2:
        Serial.print("Detection on floor ");
        curFloor = mFloor;
        mAction = 0;
        break;
      case 3:
        Serial.print("Request from down floor ");
        //destFloor = mFloor;
        mAction = 0;
        destFloor = mFloor;
        reqDirection = 0;
        req = 1;
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
        
        if((curFloor - destFloor) > 0)
        {
          liftDirection = 1;
        }else
        {
          liftDirection = 0;  
        }
      } else {
        req = 0;
      }
    }
    
    
    transmissionAddress == 5 ? transmissionAddress = 0 : transmissionAddress++;
}