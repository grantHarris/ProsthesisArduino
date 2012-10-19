#ifndef __SPOOL_VALVE_H_
#define __SPOOL_VALVE_H_

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"

class SpoolValve
{
  public:
    SpoolValve(int channelAPin, int channelBPin, int aInputPin, int bInputPin);
    void Update();
    void Dither();
    
  private:
    int mChannelAPin;
    int mAInputPin;
    int mChannelAValue;
    
    int mChannelBPin;
    int mBInputPin;
    int mChannelBValue;
};

#endif
