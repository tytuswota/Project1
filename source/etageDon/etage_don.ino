#include "ShiftOutRegister.hpp"
#include <Wire.h>

// pins
#define SHIFTREGISTER_DATA_PIN 2
#define SHIFTREGISTER_CLOCK_PIN 3
#define SHIFTREGISTER_LATCH_PIN 4
#define OPEN_LED_PIN 5
#define CLOSE_LED_PIN 6
#define WHITE_BUTTON_INPUT_PIN 7
#define WHITE_BUTTON_LED_PIN 10
#define RED_BUTTON_INPUT_PIN 8
#define RED_BUTTON_LED_PIN 9
#define REED_INPUT_PIN 11

// floor
#define FLOOR 1

// message length for message to control room
#define COM_MSG_LEN 4

// macro to display a number from 0 - 9 on the shift out register
#define disp(a) shiftreg.set(lookup[a]);shiftreg.show()

// declaring the message for the control room
char globalMessage[COM_MSG_LEN];

// lookup table for the shift out register + 7 segment display
const byte lookup[10] = {
// Q7 to Q0
  B01111110,
  B00010010,
  B10111100,
  B10110110,
  B11010010,
  B11100110,
  B11101110,
  B00110010,
  B11111110,
  B11110110
};

// class to control a led button
class LedButton {
  public:
  int state, prevstate, pin, ledPin, rising_edge, falling_edge;
  LedButton(int pin, int ledPin) {
    this->pin = pin;
    this->ledPin = ledPin;
    pinMode(pin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
  }
  void setLed(int status) {
    digitalWrite(ledPin, status);
  }
  void update() {
    state = !digitalRead(pin);
    if(state != prevstate) {
      if(state) {
        rising_edge = 1;
      } else {
        falling_edge = 1;
      }
    } else {
      rising_edge = 0;
      falling_edge = 0;
    }
    prevstate = state;
  }
};

// class to control a led
class Led {
  public:
  int pin;
  Led(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
  }
  void set(int status) {
    digitalWrite(pin, status);
  }
};

// class to control an (active low) sensor
class LiftSensor {
  public:
  int pin, state, prevstate, rising_edge, falling_edge;
  LiftSensor(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
  }
  void update() {
    state = !digitalRead(pin);
    if(state != prevstate) {
      if(state){
        rising_edge = 1;
      } else {
        falling_edge = 1;
      }
    } else {
       rising_edge = 0;
       falling_edge = 0;
    }
    prevstate = state;
  }
};

// interrupt handler to send a message to the control room
void requestHandler() {
  Serial.println("Request handler called, sending global message");
  Wire.write(globalMessage, COM_MSG_LEN);
  Wire.onRequest(NULL);
}



// function to prepare a message to send
void prepare_message(int action) {
  globalMessage[0] = action+'0';
  globalMessage[1] = ',';
  globalMessage[2] = FLOOR+'0';
  Wire.onRequest(requestHandler);
}

// request status variables
int upreq = 0, downreq = 0, req = 0;

// create an instance of a ShiftOutRegister
ShiftOutRegister shiftreg(SHIFTREGISTER_DATA_PIN, SHIFTREGISTER_CLOCK_PIN, SHIFTREGISTER_LATCH_PIN);

// create Led instances for the door led
Led open_led(OPEN_LED_PIN);
Led closed_led(CLOSE_LED_PIN);

// create LedButton instances for the buttons
LedButton white_button(WHITE_BUTTON_INPUT_PIN, WHITE_BUTTON_LED_PIN);
LedButton red_button(RED_BUTTON_INPUT_PIN, RED_BUTTON_LED_PIN);

// create a LiftSensor instance for the reed module
LiftSensor reed(REED_INPUT_PIN);

// interrupt handler to receive a message
// this function is declared after the global variables because it calls a method of the ShiftOutRegister class instance
void receiveHandler() {

  // display the number sent by the control room on the 7 segment display
  disp(Wire.read());
}

void setup() {
  
  // start values for the door LEDs
  open_led.set(0);
  closed_led.set(1);

  // display 0 until a value is sent by the control room
  disp(0);

  // initialize wire
  Wire.begin(FLOOR);
  Wire.onReceive(receiveHandler);

  Serial.begin(9600);
}

void loop() {

  // update the reed and led-buttons
  reed.update();
  white_button.update();
  red_button.update();

  // if the up-button is pressed and there is no pending request
  if(white_button.rising_edge && !req) {

    // handle the request to go up
    white_button.setLed(1);
    req = 1;
    prepare_message(1);
    
    Serial.println("Request to go up");
    Serial.print("Sending: ");
    Serial.println(globalMessage);
  }

  // if the down-button is pressed and there is no pending request
  if(red_button.rising_edge && !req) {

    // handle the request to go down
    red_button.setLed(1);
    req = 1;
    prepare_message(3);

    Serial.println("Request to go down");
    Serial.println("Sending: ");
    Serial.println(globalMessage);
  }

  // if the reed sensor goes from low to high (lift detection)
  if(reed.rising_edge) {

    // if there is a pending request
    if(req) {

      // set the request variable and LEDs to 0
      req = 0;
      white_button.setLed(0);
      red_button.setLed(0);

      // set the door LEDs to open
      open_led.set(1);
      closed_led.set(0);
      
      Serial.println("Lift arrived");
    }

    // display the current floor and prepare a 'detection' message
    disp(FLOOR);
    prepare_message(2);
    
    Serial.print("Sending: ");
    Serial.println(globalMessage);
    
    delay(50);
  }

  // if the reed goed from high to low (lift going away)
  if(reed.falling_edge) {

    // set the door LEDs to closed
    open_led.set(0);
    closed_led.set(1);
    
    Serial.println("Lift going away");
  }
}
