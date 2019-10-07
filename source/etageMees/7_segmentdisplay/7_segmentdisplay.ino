//Pin connected to ST_CP of 74HC595
int latchPin = 12;
//Pin connected to SH_CP of 74HC595
int clockPin = 13;
////Pin connected to DS of 74HC595
int dataPin = 11;
const int pinSwitch = 5; //Pin Reed
int StatoSwitch = 0;
int Ledrood = 8;
int Ledgroen = 9;
void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pinSwitch, INPUT);
  pinMode(2 , OUTPUT);
   pinMode(3 , OUTPUT);
   pinMode (Ledgroen, OUTPUT);
   pinMode (Ledrood, OUTPUT);
}

void loop() 
{
  digitalWrite (Ledgroen, LOW);
  digitalWrite (Ledrood, HIGH);
  digitalWrite (2, HIGH);
  digitalWrite (3, HIGH);
  StatoSwitch = digitalRead(pinSwitch); 
if (StatoSwitch == HIGH){
    digitalWrite (Ledrood, LOW);
  digitalWrite (Ledgroen, HIGH);
   // Decimal 91      Binary output  01011011     Displays 2
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 91);  
    digitalWrite(latchPin, HIGH);
    delay(1000);
}
else {
    
    delay(1000);

    // Decimal 63      Binary output  01111111     Displays 0
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 63);  
    digitalWrite(latchPin, HIGH);
    delay(1000);
    
} 
   
}
