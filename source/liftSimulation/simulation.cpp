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
    
    Elevator() {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                requests[i].data[j] = 0;
            }
        }
    }

    void next() {
        if(floor == 0) direction = UP;
        else if(floor == 4) direction = DOWN;
        floor += direction;
        printf("lift going %c, now on floor %d\n", dir_to_char(direction), floor);
        for(int i = 0; i < 4; i++) {
            if(requests[i].source == floor) {
                requests[i].stop_passed = 1;
            }
        }
        update();
    }

    void update() {
        if(requests[floor].type == direction || (requests[floor].type == STOP && requests[floor].stop_passed)) {
            requests[floor].type = NONE;
            printf("stopping at floor %d\n", floor);
        }
    }

    void make_request(int floor, int dir) {
        requests[floor].type = dir;
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
    elevator.floor = 0;

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