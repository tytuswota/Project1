#include <Arduino.h>

class MessageDecoder{
    char encodedMessage[], decodeMessage[];
    public:
    MessageDecoder(String msg);
    void setMessage(char encodedMessage[]);
    char getMessage(char index);
      
};