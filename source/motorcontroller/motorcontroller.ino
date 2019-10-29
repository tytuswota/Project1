class ControlledMotor {
  public:
  int pin1, pin2;
  ControlledMotor(int pin1, int pin2) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }
  void spina() {
    digitalWrite(pin1, 1);
    digitalWrite(pin2, 0);
  }
  void spinb() {
    digitalWrite(pin1, 0);
    digitalWrite(pin2, 1);
  }
  void stop() {
    digitalWrite(pin1, 0);
    digitalWrite(pin2, 0);
  }
};

ControlledMotor motor(5, 6);

void setup() {
  
}


void loop() {
  motor.spina();
  //delay(2000);
  //motor.stop();
  delay(2000);
  motor.spinb();
  //delay(2000);
  //motor.stop();
  delay(2000);
}
