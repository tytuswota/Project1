#include "ShiftOutRegister.hpp"

ShiftOutRegister::ShiftOutRegister(int dp, int cp, int lp) {
  dataPin = dp;
  clockPin = cp;
  latchPin = lp;
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

void ShiftOutRegister::setResetPin(int pin) {
  resetPin = pin;
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, 1);
}

void ShiftOutRegister::setOutputEnablePin(int pin) {
  outputEnablePin = pin;
  pinMode(outputEnablePin, OUTPUT);
  digitalWrite(outputEnablePin, 0);
}

void ShiftOutRegister::clockPulse() {
  digitalWrite(clockPin, 1);
  digitalWrite(clockPin, 0);
}

void ShiftOutRegister::show() {
  digitalWrite(latchPin, 1);
  digitalWrite(latchPin, 0);
}

void ShiftOutRegister::set(byte data) {
  shiftOut(dataPin, clockPin, MSBFIRST, data);
}

void ShiftOutRegister::reset() {
  digitalWrite(resetPin, 0);
  digitalWrite(resetPin, 1);
}

void ShiftOutRegister::disableOutput() {
  digitalWrite(outputEnablePin, 1);
}

void ShiftOutRegister::enableOutput() {
  digitalWrite(outputEnablePin, 0);
}