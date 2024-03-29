
#include "ShiftOutRegister.hpp"


const byte lookup[10] = {
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

ShiftOutRegister shiftreg(2, 4, 3);




void setup() {
  //shiftreg.setResetpin(6);
  //shiftreg.setOutputEnablePin(5);
  pinMode(5, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < 10; i++) {
    int detection = !digitalRead(5);
    Serial.println(detection);
    

    if(detection) {
      shiftreg.set(lookup[i]);
      shiftreg.show();
      delay(1000);
    } else {
      i--;
    }

    
  }

}
