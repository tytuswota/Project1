#include <Wire.h>
#include <Keypad.h>

#define FLOORCOUNT 4
#define LOWEST_FLOOR 1
#define HIGHEST_FLOOR 4

#define NONE 0
#define UP 1
#define DETECTION 2
#define DOWN 3

#define CALIBRATION_TIMEOUT 4000

enum elevatorstate {
  main, calibrating, spinning
};

class MotorController {
  public:
  int pin_a, pin_b;
  int up_stops[FLOORCOUNT], down_stops[FLOORCOUNT];
  int current_floor;
  bool detection;
  int direction;
  int dest;
  int calibration_endtime;
  enum elevatorstate state;
  
  MotorController(int pin_a, int pin_b) {
    this->pin_a = pin_a;
    this->pin_b = pin_b;
    pinMode(pin_a, OUTPUT);
    pinMode(pin_b, OUTPUT);
    memset(up_stops, 0, FLOORCOUNT);
    memset(down_stops, 0, FLOORCOUNT);
    this->detection = false;
  }

  void make_stop_request(int dstfloor) {
    if(dstfloor > floor) {
      up_stops[dstfloor] = 1;
    } else if(dstfloor < floor) {
      down_stops[dstfloor] = 1;
    }
  }

  void spin(int direction) {
    switch(direction) {
      case UP:
        digitalWrite(pin_a, 1);
        digitalWrite(pin_b, 0);
        break;
      case DOWN:
        digitalWrite(pin_a, 0);
        digitalWrite(pin_b, 1);
        break;
      case NONE:
        digitalWrite(pin_a, 0);
        digitalWrite(pin_b, 0);
        break;
      default:
        Serial.println("that's not a direction!");
        break;
    }
  }

  int pending_request() {
    for(int i = 0; i < FLOORCOUNT; i++) {
      if(up_stops[i]) return UP;
      else if(down_stops[i]) return DOWN;
    }
    return 0;
  }

  void send_action(int srcfloor, int type) {
    switch(type) {
      case UP:
        Serial.println("UP request made at floor " + (String)srcfloor);
        up_stops[srcfloor] = 1;
        break;
      case DOWN:
        Serial.println("DOWN request made at floor " + (String)srcfloor);
        down_stops[srcfloor] = 1;
        break;
      case DETECTION:
        Serial.println("DETECTION at floor " + (String)srcfloor);
        current_floor = srcfloor;
        detection = true;
        break;
      default:
        Serial.println("that's not a valid action you absolute idiot");
        break;
    }
  }
};

char keys[4][4] = {
  {'1', '2', '3', 0},
  {'4', 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

byte rowPins[4] = {4, 5, 6, 7};
byte colPins[4] = {8, 9, 10, 11};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

char wirebuffer[255];
byte bpos = 0;
bool newdata = false;

MotorController motorcontroller = MotorController(2, 3);

void setup() {
  Serial.begin(9600);
  Wire.begin(0);
  Wire.onReceive(receive);
  memset(wirebuffer, 0, 255);
  motorcontroller.state = calibrating;
  motorcontroller.calibration_endtime = millis()+CALIBRATION_TIMEOUT;
}

void loop() {
  char in = keypad.getKey();
  if(in) {
    Serial.println("received keypad input: " + (String)in);
  }
  switch(motorcontroller.state) {
    case calibrating:
      if(millis() > motorcontroller.calibration_endtime) {
        motorcontroller.spin(DOWN);
      } else {
        motorcontroller.spin(UP);
      }
      Serial.println("now in calibrating");
      if(motorcontroller.detection) {
        motorcontroller.state = main;
      }
      break;
    case main:
      motorcontroller.spin(NONE);
      Serial.println("now in main");
      break;
  }
}

void receive() {
  memset(wirebuffer, 0, 255);
  while(Wire.available()) {
    wirebuffer[bpos++] = Wire.read();
  }
  bpos = 0;
  motorcontroller.send_action(wirebuffer[2]-'0', wirebuffer[0]-'0');
}
