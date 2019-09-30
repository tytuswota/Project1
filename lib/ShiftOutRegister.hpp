/*
simple library to control a single shift-out register
author: Don Luijendijk
*/

#include <Arduino.h>

class ShiftOutRegister {
  int clockPin, dataPin, latchPin, resetPin, outputEnablePin;
  public:
  ShiftOutRegister(int dp, int cp, int lp);
  
  void setResetPin(int pin);
  void setOutputEnablePin(int pin);
  void clockPulse();
  void show();
  void set(byte data);
  void reset();
  void disableOutput();
  void enableOutput();
};