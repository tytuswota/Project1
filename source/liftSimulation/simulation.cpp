#include <iostream>
using namespace std;

#define DOWN -1
#define NONE 0
#define UP 1
#define STOP 2

typedef union req {
    struct {
        int type, source, stop_passed;
    };
    int data[3];
}req;

// function to turn a direction in a char for printing
char dir_to_char(int dir) {
    char arr[] = "v-^";
    return arr[dir+1];
}

class Elevator {
    public:
    int direction;
    int floor;
    int prevreqsrc;
    req requests[4];
    
    // constructor sets all request data to zeroes
    Elevator() {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                requests[i].data[j] = 0;
            }
        }
    }

    // function to go to the next floor
    void next() {
        if(floor == 1) direction = UP;
        else if(floor == 4) direction = DOWN;
        floor += direction;
        printf("lift going %c, now on floor %d\n", dir_to_char(direction), floor);
        handle_stops();
    }

    // functon to handle stopping
    void handle_stops() {

        // check if there is a request with the current elevator floor as source
        for(int i = 0; i < 4; i++) {
            if(requests[i].source == floor) {

                // set the request to 'passed'
                requests[i].stop_passed = 1;
            }
        }

        // check if there's a (passed) stop request or request to go in the current direction of the elevator
        if(requests[floor].type == direction || (requests[floor].type == STOP && requests[floor].stop_passed)) {
            
            // handle the request
            requests[floor].type = NONE;
            printf("stopping at floor %d\n", floor);
        }
    }

    // function to make a request
    void make_request(int floor, int dir) {
        requests[floor].type = dir;

        // if the request is a stop request, set the source as the floor from
        // the previous request
        if(dir == STOP) {
            requests[floor].source = prevreqsrc;
        }
        prevreqsrc = floor;
    }
};

int main() {
    Elevator elevator;

    // start at floor 0
    // should be set to the floor sending a detection
    // if no floor is sending a detection, go (up/down?) until a floor is sending a detection
    elevator.floor = 1;

    // start by going down if the current floor is 4, else start by going up
    if(elevator.floor == 4) {
        elevator.direction = DOWN;
    } else {
        elevator.direction = UP;
    }

    // floor 1 wants to go to floor 3
    elevator.make_request(1, UP);
	elevator.make_request(3, STOP);

    elevator.make_request(3, DOWN);
    elevator.make_request(2, STOP);

    // simulate some iterations
    for(int i = 0; i < 10; i++) {
        elevator.next();
    }
}