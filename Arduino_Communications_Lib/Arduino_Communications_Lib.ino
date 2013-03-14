#include <Arduino.h>
#include "command_processor.h"

//#define DEBUG_REMAINING_INPUT_STREAM 1

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

const char* kArduinoID = "test";

CommandProcessor sProcessor(256);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  
}

void serialEvent()
{
  while (Serial.available())
  {
     sProcessor.AddChar((char)Serial.read());
  }
  
  if (sProcessor.HasCommand())
  {
    Serial.println("Got one!!");
    int buffLen;
    const char *buff = sProcessor.GetBuffer(buffLen);
    
    #if DEBUG_REMAINING_INPUT_STREAM
    char msg[256];
    strncpy(msg, buff, buffLen);
    if (buffLen > 0)
    {
       msg[buffLen] = '\0';
    }
    else
    {
      msg[0] = '\0'; 
    }
    
    Serial << "Buffer is " << msg << "\n";
    #endif
    
    CommandPacket *test = sProcessor.GetCommand();
    if (test != NULL)
    {
      free(test);
    }
    
    #if DEBUG_REMAINING_INPUT_STREAM
    buff = sProcessor.GetBuffer(buffLen);
    strncpy(msg, buff, buffLen);
    if (buffLen > 0)
    {
       msg[buffLen] = '\0';
    }
    else
    {
      msg[0] = '\0'; 
    }
    
    Serial << msg << "\nBuff length was " << buffLen << "\n";
    #endif
  }
  
}
