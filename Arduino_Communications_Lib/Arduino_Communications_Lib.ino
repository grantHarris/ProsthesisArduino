#include <Arduino.h>
#include "command_processor.h"

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
    CommandPacket *test = sProcessor.GetCommand();
    free(test);
    
    int buffLen;
    const char *buff = sProcessor.GetBuffer(buffLen);
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
    Serial.println(msg);
  }
  
}
