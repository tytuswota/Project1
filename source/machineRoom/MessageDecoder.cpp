#include <string.h>
#include "MessageDecoder.hpp"


MessageDecoder::MessageDecoder(String msg);
{
    char buf[msg.length()+1];
    msg.toCharArray(buf, sizeof(buf));
    char *element;
    char *pter = buf;

    while((element = strtok_r(pter, ",", &pter)))
    {
        
    }

}

MessageDecoder::setMessage(char encodeMessage[])
{
    encodeMessage = encodeMessage[];
}

MessageDecoder::getMessage(char index)
{
    return encodeMessage;
}