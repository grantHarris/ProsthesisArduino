#ifndef __SPOOL_VALVE_H_
#define __SPOOL_VALVE_H_

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"

class SpoolValve
{
  public:
    SpoolValve(int channelAPin, int channelBPin);
    void Update();
    
  private:
    int mChannelAPin;
    int mChannelBPin;
};

#endif
