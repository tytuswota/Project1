#include <Wire.h>
#include <Keypad.h>

// actions and directions
#define NONE 0
#define UP 1
#define DETECT 2
#define DOWN 3

// floor info
#define FLOORCOUNT 4
#define LOWESTFLOOR 1
#define HIGHESTFLOOR 4
#define INVALIDFLOOR -1

#define CHAR_RECEIVE_AMOUNT 4

#define CALIBRATION_TIMEOUT 4000

int action, task_floor;

class Motor {
  public:

  // buffer for receiving data from i2c
  char buffer[255];
  byte bpos = 0;

  // variables to store information about requests and actions
  bool newtask = false;

  int transmissionAddress;
  
  int pin_a, pin_b;
  int detection;
  int task_floor_good;
  int currentfloor;

  // constructor
  Motor(int pin_a, int pin_b) {
    this->pin_a = pin_a;
    this->pin_b = pin_b;
    pinMode(pin_a, OUTPUT);
    pinMode(pin_b, OUTPUT);
    currentfloor = INVALIDFLOOR;
    detection = INVALIDFLOOR;
  }
  //updates the communication status and the status of the motor
  void update()
  {
      for(transmissionAddress = LOWESTFLOOR; transmissionAddress <= HIGHESTFLOOR; transmissionAddress++)
      {
        Wire.beginTransmission(transmissionAddress);
        Wire.write(detection);
        Wire.endTransmission();

        Wire.requestFrom(transmissionAddress, CHAR_RECEIVE_AMOUNT);

        // clear and refill the buffer
        memset(buffer, 0, 255);
        bpos = 0;
        while (Wire.available()) {
          buffer[bpos++] = Wire.read();
        }
        
        //Serial.println(task_floor);
        action = buffer[0]-'0';
        //Serial.println(action);
        // get the floor and action
        task_floor = buffer[2]-'0';
        
        
        if(task_floor > HIGHESTFLOOR || task_floor < LOWESTFLOOR) {
          //Serial.println("received invalid floor: " + (String)task_floor);

          
        }
      
        // if it's 1 or 3, it is a task to go to a floor else it is a detection
        if(action == 1 || action == 3) {
          newtask = true;
          task_floor_good = task_floor;
        } else if(action == 2) {
          detection = task_floor;
          Serial.println("detection at floor " + (String)task_floor);
        }
      }  
  }

  // function to spin the motor, used with h-bridge
  void spin(int dir) {
    switch(dir) {
      case UP:
        digitalWrite(pin_a, 0);
        digitalWrite(pin_b, 1);
        break;
      case DOWN:
        digitalWrite(pin_a, 1);
        digitalWrite(pin_b, 0);
        break;
      case NONE:
        digitalWrite(pin_a, 0);
        digitalWrite(pin_b, 0);
        break;
      default:
        Serial.println("error: 'spin' function got invalid direction");
    }
  }

  // function to go to a floor, will hang the arduino if it does not detect anything
  void gotofloor() {
    int dest = task_floor_good;
    // direction to spin in
    int spin_dir = (dest > currentfloor) ? UP : DOWN;

    // while the lift isnt at dest
    while(detection != dest) {
      update();
      // spin the motor
      spin(spin_dir);
      Serial.println("going to floor " + (String)dest);
      delay(2);
    }

    // set the new floor and stop spinning
    currentfloor = dest;
    spin(NONE);
    Serial.println("Gotofloor: arrived at floor " + (String)currentfloor);
  }

  // function to put the lift at a floor and not inbetween two floors
  void calibrate() {

    // timeout to spin in the other direction
    int endtime = millis()+CALIBRATION_TIMEOUT;

    // keep iterating until the lift is at a floor
    while(currentfloor == INVALIDFLOOR) {
      //look it up with crl f
      update();
      Serial.println("Calibrating");

      // check if there are detections
      currentfloor = detection;

      // start by going down, go up after CALIBRATION_TIMEOUT ms
      // useful when the lift is below the last floor and not sending a signal for some reason
      if(endtime > millis()) {
        spin(UP);
      } else {
        spin(DOWN);
      }
      delay(2);
    } 

    spin(NONE);
    Serial.println("Calibration: arrived at floor " + (String)currentfloor);
  }
};

// keypad data
char keys[4][4] = {
  {'1', '2', '3', 0},
  {'4', 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

byte rowPins[4] = {4, 5, 6, 7};
byte colPins[4] = {8, 9, 10, 11};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

// motor with h-bridge at pin 2 and 3
Motor motor(2, 3);

void setup() {

  Serial.begin(9600);

  // clear the input buffer
  memset(motor.buffer, 0, 255);

  // start wire with a handler function
  Wire.begin(0);
  //Wire.onReceive(receive);

  // calibrate the motor so the floor can be set
  motor.calibrate();

}

void loop() {
  motor.update();
  // get keypad input, if there is input set it as new task
  char in = keypad.getKey();
  if(in) {
    motor.newtask = true;
    motor.task_floor_good = in-'0';
  }

  // go to task_floor if there is a new task
  if(motor.newtask) {
    Serial.println("Received task: go to " + (String)task_floor);
    Serial.println("new task: " + (String)task_floor);
    motor.gotofloor();
    motor.newtask = false;
  }
}

// handler for when data is sent
/*void receive() {

  
}*/
