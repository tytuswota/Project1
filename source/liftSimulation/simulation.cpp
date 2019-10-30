#include <iostream>
using namespace std;

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

class Elevator {
    public:

    int direction;
    int floor;
    int prev_req_source;
    req requests[4];
    
    // constructor sets all data in 'requests' zeroes
    Elevator() {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                requests[i].data[j] = 0;
            }
        }
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

            // go up/down (depends on direction)
            floor += direction;

            printf("lift going %c, now on floor %d\n", dir_to_char(direction), floor);

        // else do nothing
        } else {
            printf("No more pending requests, the lift is at floor %d.\n", floor);
        }
    }

    // functon to handle stopping
    void handle_stops() {

        // check if there's a (passed) stop request or request to
        // go in the current direction of the elevator
        if(requests[floor].type == direction ||
           (requests[floor].type == STOP &&
            requests[floor].stop_passed)) {
            
            // handle the request
            requests[floor].type = NONE;
            printf("stopping at floor %d\n", floor);
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
                //printf("Pending request: %d at floor %d\n", requests[i].type, i);
                return 1;
            }
        }
        return 0;
    }

    // function to simulate iterations
    void simulate_iterations(int num) {
        for(int i = 0; i < num; i++) {
            next();
            //printf("\n");
        }
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

int main() {
    Elevator elevator;

    // start at floor 0
    // should be set to the floor sending a detection
    // if no floor is sending a detection, go (up/down?) until a floor is sending a detection
    elevator.floor = 1;

    elevator.set_direction();

    // floor 1 wants to go to floor 3
    elevator.make_request(1, UP);
	elevator.make_request(3, STOP);

    // floor 3 wants to go to floor 2
    elevator.make_request(3, DOWN);
    elevator.make_request(2, STOP);

    // simulate some iterations
    elevator.simulate_iterations(10);

    // make a new request
    elevator.make_request(4, DOWN);
    elevator.make_request(2, STOP);

    // simulate some more iterations
    elevator.simulate_iterations(10);
}