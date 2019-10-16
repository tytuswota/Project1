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
#define FLOOR 3
#define COM_MSG_LEN 4

#define disp(a) shiftreg.set(lookup[a]);shiftreg.show()

char globalMessage[COM_MSG_LEN];

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

void requestHandler() {
  Serial.println("Request handler called, sending global message");
  Wire.write(globalMessage, COM_MSG_LEN);
  Wire.onRequest(NULL);
}

void message(int action) {
  globalMessage[0] = action+'0';
  globalMessage[1] = ',';
  globalMessage[2] = FLOOR+'0';
  Wire.onRequest(requestHandler);
}

int upreq = 0, downreq = 0, req = 0;
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
  Wire.begin(FLOOR);
  Serial.begin(9600);
}

void loop() {
  reed.update();
  white_button.update();
  red_button.update();
  
  if(white_button.rising_edge && !req) {
    white_button.setLed(1);
    upreq = 1;
    Serial.println("Request to go up");
    message(1);
    Serial.print("Sending: ");
    Serial.println(globalMessage);
  }
  if(red_button.rising_edge && !req) {
    red_button.setLed(1);
    downreq = 1;
    Serial.println("Request to go down");
    message(1);
    Serial.println(globalMessage);
  }
  req = upreq || downreq;
  if(reed.rising_edge) {
    if(req) {
      upreq = 0;
      downreq = 0;
      white_button.setLed(0);
      red_button.setLed(0);
      open_led.set(1);
      closed_led.set(0);
      disp(FLOOR);
      Serial.println("Lift arrived");
    }
    message(2);
    Serial.print("Sending: ");
    Serial.println(globalMessage);
    delay(50);
  }
  if(reed.falling_edge) {
    open_led.set(0);
    closed_led.set(1);
    disp(0);
    Serial.println("Lift going away");
  }
}
