#include "ShiftOutRegister.hpp"

// constructor which uses the data pin, clock pin and latch pin as parameters
ShiftOutRegister::ShiftOutRegister(int dp, int cp, int lp) {
  dataPin = dp;
  clockPin = cp;
  latchPin = lp;
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

// optional function to add a reset pin
void ShiftOutRegister::setResetPin(int pin) {
  resetPin = pin;
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, 1);
}

// optional function to add an output-enable pin
void ShiftOutRegister::setOutputEnablePin(int pin) {
  outputEnablePin = pin;
  pinMode(outputEnablePin, OUTPUT);
  digitalWrite(outputEnablePin, 0);
}

// this function pulses the latch pin to set the output to the stored byte
void ShiftOutRegister::show() {
  digitalWrite(latchPin, 1);
  digitalWrite(latchPin, 0);
}

// this function stores a byte on the shift register
void ShiftOutRegister::set(byte data) {
  shiftOut(dataPin, clockPin, MSBFIRST, data);
}

// this function resets the storage
// this function only works when the reset pin has been set
void ShiftOutRegister::reset() {
  digitalWrite(resetPin, 0);
  digitalWrite(resetPin, 1);
}

// this function sets the output to zeroes without overwriting the output latches
void ShiftOutRegister::disableOutput() {
  digitalWrite(outputEnablePin, 1);
}

// this function sets the output back to whatever is in the output latches
void ShiftOutRegister::enableOutput() {
  digitalWrite(outputEnablePin, 0);
}