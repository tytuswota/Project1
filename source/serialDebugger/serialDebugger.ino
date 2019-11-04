#include <Wire.h>

char buffer[255];
byte bpos = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  memset(buffer, 0, 255);
}

void loop() {
  while(Serial.available()) {
    buffer[bpos++] = Serial.read();
    delay(2);
  }
  if(buffer[0]) {
    Wire.beginTransmission(0);
    strtok(buffer, "\n");
    Serial.println(buffer);
    Wire.write(buffer, 3);
    Wire.endTransmission();
    memset(buffer, 0, 255);
    bpos = 0;
  }
}