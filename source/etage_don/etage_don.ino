#include "ShiftOutRegister.hpp"
#include "Protocol.hpp"

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
#define FLOOR 1

#define disp(a) shiftreg.set(lookup[a]);shiftreg.show()

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

int upreq = 0, downreq = 0;
ShiftOutRegister shiftreg(SHIFTREGISTER_DATA_PIN, SHIFTREGISTER_CLOCK_PIN, SHIFTREGISTER_LATCH_PIN);
Led open_led(OPEN_LED_PIN);
Led closed_led(CLOSE_LED_PIN);
LedButton white_button(WHITE_BUTTON_INPUT_PIN, WHITE_BUTTON_LED_PIN);
LedButton red_button(RED_BUTTON_INPUT_PIN, RED_BUTTON_LED_PIN);
LiftSensor reed(REED_INPUT_PIN);
Protocol communication(FLOOR);

void setup() {
  open_led.set(0);
  closed_led.set(1);
  disp(0);
  communication.setFloor(FLOOR);
  Serial.begin(9600);
}

void loop() {
  reed.update();
  white_button.update();
  red_button.update();
  
  if(white_button.rising_edge) {
    white_button.setLed(1);
    red_button.setLed(0);
    upreq = 1;
    downreq = 0;
    Serial.println("Request to go up");
    communication.sendCallSignal();
  }
  if(red_button.rising_edge) {
    red_button.setLed(1);
    white_button.setLed(0);
    downreq = 1;
    upreq = 0;
    Serial.println("Request to go down");
    communication.sendCallSignal();
  }
  if(reed.rising_edge && (upreq || downreq)) {
    upreq = 0;
    downreq = 0;
    white_button.setLed(0);
    red_button.setLed(0);
    open_led.set(1);
    closed_led.set(0);
    disp(FLOOR);
    Serial.println("Lift arrived");
    communication.sendDetectionSignal();
    delay(50);
  }
  if(reed.falling_edge && !reed.prevstate) {
    open_led.set(0);
    closed_led.set(1);
    disp(0);
    Serial.println("Lift going away");
  }
}
