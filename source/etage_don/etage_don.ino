#include "ShiftOutRegister.hpp"

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

int request = 0;
ShiftOutRegister shiftreg(2, 3, 4);
LedButton white_button(7, 10);
LedButton red_button(8, 9);
LiftSensor reed(11);
Led open_led(5);
Led closed_led(6);

void setup() {
  open_led.set(0);
  closed_led.set(1);
  disp(0);
  Serial.begin(9600);
}

void loop() {
  reed.update();
  white_button.update();
  red_button.update();
  
  if(white_button.rising_edge) {
    white_button.setLed(1);
    red_button.setLed(0);
    request = 1;
    Serial.println("Request to go up");
  }
  if(red_button.rising_edge) {
    red_button.setLed(1);
    white_button.setLed(0);
    request = 1;
    Serial.println("Request to go down");
  }
  if(reed.rising_edge && request) {
    request = 0;
    white_button.setLed(0);
    red_button.setLed(0);
    open_led.set(1);
    closed_led.set(0);
    disp(1);
    Serial.println("Lift arrived");
    delay(50);
  }
  if(reed.falling_edge && !reed.prevstate) {
    open_led.set(0);
    closed_led.set(1);
    disp(0);
    Serial.println("Lift going away");
  }
}
