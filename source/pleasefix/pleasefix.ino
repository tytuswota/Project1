#define DOWN -1
#define NONE 0
#define UP 1
#define STOP 2

// req has a type, a source and a variable to see if the source has been passed
typedef union req {
    struct {
        int type, source, stop_passed;
    };
    int data[3];
}req;

// function to turn a direction into a char for printing
char dir_to_char(int dir) {
    char arr[] = "v-^x";
    return arr[dir+1];
}

class ControlledMotor {
  public:
  int pin1, pin2;
  ControlledMotor(int pin1, int pin2) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }

  ControlledMotor() {}

  void spin(int dir) {
    switch(dir) {
      case 0:
        digitalWrite(pin1, 0);
        digitalWrite(pin2, 0);
        break;
      case 1:
        digitalWrite(pin1, 1);
        digitalWrite(pin2, 0);
        break;
      case -1:
        digitalWrite(pin1, 0);
        digitalWrite(pin2, 1);
        break;
    }
  }
};

class Elevator {
    public:

    int direction;
    int floor;
    int endtime;
    int prev_req_source;
    req requests[4];
    ControlledMotor m;
    
    // constructor sets all data in 'requests' zeroes
    Elevator(int motor_pin_1, int motor_pin_2) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                requests[i].data[j] = 0;
            }
        }
        m = ControlledMotor(motor_pin_1, motor_pin_2);
    }

    // function to go to the next floor
    void next() {

        // do things if there's a pending request
        if(pending_requests()) {

            // handle passing floors
            handle_pass();

            // handle stops
            handle_stops();

            // change the direction if the elevator is on the first or last floor
            if(floor == 1) direction = UP;
            else if(floor == 4) direction = DOWN;
            
            Serial.println("lift going " + (String)dir_to_char(direction) + ", to floor " + (String)floor);
            
            // go up/down (depends on direction)
            floor += direction;
            m.spin(direction);
            delay(2000);
            m.spin(0);
            delay(500);

        // else do nothing
        } else {
            Serial.println("No more pending requests, the lift is at floor " + (String)floor);
        }
    }

    // functon to handle stopping
    void handle_stops() {

        // check if there's a (passed) stop request or request to
        // go in the current direction of the elevator
        if((requests[floor].type == direction || (requests[4].type && floor == 4) || (requests[1].type && floor == 1)) || (requests[floor].type == STOP && requests[floor].stop_passed)) {
            
            // handle the request
            requests[floor].type = NONE;
            Serial.println("Stopping at floor " + (String)floor);
            delay(4000);
        }
    }

    // function to handle passing a floor
    void handle_pass() {

        // check if there is a request with the current elevator floor as source
        for(int i = 0; i < 4; i++) {
            if(requests[i].source == floor && requests[i].type == STOP) {

                // set the request to 'passed'
                requests[i].stop_passed = 1;
            }
        }
    }

    // function to make a request
    void make_request(int floor, int dir) {

        // check if there is no stop request (stop requests can't be overridden)
        if(requests[floor].type != STOP) {
            requests[floor].type = dir;
        }

        // if the request is a stop request, set the source as the floor from
        // the previous request
        if(dir == STOP) {
            requests[floor].source = prev_req_source;
        }
        requests[floor].stop_passed = 0;
        prev_req_source = floor;
    }

    // check if there are any pending requests
    int pending_requests() {

        // iterate through all requests
        for(int i = 0; i < 4; i++) {

            // check if the request is not 0 (NONE)
            if(requests[i].type) {
                return 1;
            }
        }
        return 0;
    }

    // update the direction, this should always be called after manually
    // setting the floor
    void set_direction() {
        if(floor == 4) {
            direction = DOWN;
        } else {
            direction = UP;
        }
    }
};

Elevator e(5, 6);

void setup() {
  Serial.begin(9600);
  
  e.floor = 1;
  e.make_request(1, UP);
  e.make_request(3, STOP);

  e.make_request(3, DOWN);
  e.make_request(2, STOP);

  for(int i = 0; i < 10; i++) {
    e.next();
  }

  e.make_request(4, DOWN);
  e.make_request(3, STOP);

  for(int i = 0; i < 10; i++) {
    e.next();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
