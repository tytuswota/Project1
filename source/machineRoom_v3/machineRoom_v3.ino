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

int cur_spinning_direction;
int transmission_adress = 0;
static unsigned long wait_on_floor = millis();

class MotorController {
  public:
  int pin_a, pin_b;
  int up_stops[FLOORCOUNT], down_stops[FLOORCOUNT];
  int current_floor;
  bool detection;
  //int direction;
  //int dest;
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

  //should take the current_floor as index for the array;
  void next_floor(int dir)
  {
    //Serial.println("========in the next floor function START========");

    if((current_floor == 4) && (dir == UP))
    {
      dir = DOWN;
    }
    
    if((current_floor == 1) && (dir == DOWN))
    {
      dir = UP;
    }
    
    if(dir == UP)
    {
      Serial.println("in the function up if");
      if(up_stops[current_floor])
      {
        up_stops[current_floor] = 0;
        Serial.println("in the direction stop");
        
        spin(NONE);
        
        //wait_on_floor = ((unsigned long) millis() + 4000);
        wait_on_floor = millis();
        
        this->state = main;
      }else
      {
        spin(UP);
      }
    }
    
    if(dir == DOWN)
    {
      Serial.println("in the function DOWN if");
      if(down_stops[current_floor])
      {
        down_stops[current_floor] = 0;
        Serial.println("in the direction stop");
        
        spin(NONE);

        wait_on_floor = millis();
        //wait_on_floor = ((unsigned long) millis() + 4000);
        
        this->state = main;
      }else
      {
        spin(DOWN);
      }
    }
    //Serial.println("========in the next floor function END========");
  }

  void make_stop_request(int dstfloor) 
  {
    if(dstfloor > floor) 
    {
      up_stops[dstfloor] = 1;
    } else if(dstfloor < floor) 
    {
      down_stops[dstfloor] = 1;
    }
  }

  void spin(int direction) 
  {
    switch(direction) 
    {
      case UP:
        //Serial.println("going up");
        digitalWrite(pin_a, 1);
        digitalWrite(pin_b, 0);
        break;
      case DOWN:
        //Serial.println("going down");
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
    for(int i = 0; i < FLOORCOUNT; i++) 
    {
      Serial.println("pending up: " + (String) up_stops[i]);
      Serial.println("pending down: " + (String) down_stops[i]);
      if(up_stops[i]) return UP;
      else if(down_stops[i]) return DOWN;
    }
    return 0;
  }

  void send_action(int srcfloor, int type)
  {
    switch(type) 
    {  
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

  wait_on_floor = millis();
}

void loop() {
  //sends the current possition to the slaves
  /*transmissionAddress == 4 ? transmissionAddress = 1 : transmissionAddress++;
  Wire.beginTransmission(transmissionAddress);
  Wire.write(curFloor);
  Wire.endTransmission();
    
  Wire.requestFrom(transmissionAddress, CHAR_RECEIVE_AMOUNT);
  memset(buffer, 0, CHAR_RECEIVE_AMOUNT);*/
  
  char in = keypad.getKey();
  if(in) {
    Serial.println("received keypad input: " + (String)in);
  }
  switch(motorcontroller.state) {
    
    case calibrating:
      if(millis() > motorcontroller.calibration_endtime) 
      {
        motorcontroller.spin(DOWN);
      } else 
      {
        motorcontroller.spin(UP);
      }
      Serial.println("now in calibrating");
      if(motorcontroller.detection) {
        motorcontroller.state = main;
      }
      break;
      
    case main:
      ///hier
      motorcontroller.spin(NONE);
      Serial.println("the wait on floor var");
      Serial.println((String)wait_on_floor);
      
      if(millis() - wait_on_floor < 4000) 
      {
        motorcontroller.spin(NONE);
        //motorcontroller.spin(DOWN);
      } else 
      {
        //motorcontroller.spin(UP);
        cur_spinning_direction = motorcontroller.pending_request();
        Serial.println(cur_spinning_direction);
        if(cur_spinning_direction != 0)
        {
          motorcontroller.state = spinning;
        }
      }
      break;
      
    case spinning:
      //Serial.println(cur_spinning_direction);
      motorcontroller.next_floor(cur_spinning_direction);
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
