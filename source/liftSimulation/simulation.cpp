#include <iostream>
using namespace std;

#define DOWN -1
#define NONE 0
#define UP 1
#define STOP 2

#define FLOORCOUNT 4
#define FIRSTFLR 1
#define LASTFLR 4

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

    int elevator_direction;
    int floor;
    int prev_req_source;
    req requests[FLOORCOUNT];
    
    // constructor sets all data in 'requests' zeroes
    Elevator() {
        for(int i = 0; i < FLOORCOUNT; i++) {
            for(int j = 0; j < FLOORCOUNT; j++) {
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

            // change the direction if the elevator is on the first or
            // last floor
            if(floor == FIRSTFLR) elevator_direction = UP;
            else if(floor == LASTFLR) elevator_direction = DOWN;

            // go up/down (depends on direction)
            floor += elevator_direction;

            printf("lift going %c, ", dir_to_char(elevator_direction));
            printf("now on floor %d\n", floor);

        // else do nothing
        } else {
            printf("No more pending requests, ");
            printf("the lift is at floor %d.\n", floor);
        }
    }

    // functon to handle stopping
    void handle_stops() {

        // check if there's
        // - either a request to go in the same direction as the lift at
        //   the current floor
        // - or a request and the floor is the first or last
        // - or a (passed) stop request at the current floor
        if((requests[floor].type == elevator_direction)
        || ((requests[floor].type) && (floor == LASTFLR || floor == FIRSTFLR))
        || ((requests[floor].type == STOP) && (requests[floor].stop_passed))) {

            // make a stop
            requests[floor].type = NONE;
            printf("stopping at floor %d\n", floor);
        }
    }

    // function to handle passing a floor
    void handle_pass() {

        // check if there is a request with the current elevator floor as source
        for(int i = 0; i < FLOORCOUNT; i++) {
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
        for(int i = 0; i < FLOORCOUNT; i++) {

            // check if the request is not 0 (NONE)
            if(requests[i].type) {
                return 1;
            }
        }
        return 0;
    }

    // function to simulate iterations
    void simulate_iterations(int num) {
        for(int i = 0; i < num; i++) {
            next();
        }
    }
};

int main() {
    Elevator elevator;

    // start at floor 1
    // should be set to the floor sending a detection
    // if no floor is sending a detection, go (up/down?)
    // until a floor is sending a detection
    elevator.floor = 1;

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