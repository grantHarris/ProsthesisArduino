#include "spool_valve.h"

const int inputDeadZone = 190;

SpoolValve::SpoolValve(int channelAPin, int channelBPin, int aInputPin, int bInputPin)
: 
mChannelAPin(channelAPin),
mChannelBPin(channelBPin),
mAInputPin(aInputPin),
mBInputPin(bInputPin) {}

void SpoolValve::Update()
{
    // set the LED with the ledState of the variable:
    mChannelAValue = analogRead(mAInputPin);
    mChannelBValue = analogRead(mBInputPin);  
    
    if (mChannelAValue <= inputDeadZone)
    {
      mChannelAValue = 0;
      mChannelBValue -= inputDeadZone;
      mChannelBValue = max(0, (float)255 * (float)mChannelBValue / ((float)1024 - (float)inputDeadZone));
    }
    else if (mChannelBValue <= inputDeadZone)
    {
      mChannelBValue = 0; 
      mChannelAValue -= inputDeadZone;
      mChannelAValue = max(0, (float)255 * (float)mChannelAValue / ((float)1024 - (float)inputDeadZone));
    }
    
    analogWrite(mChannelAPin, mChannelAValue);
    analogWrite(mChannelBPin, mChannelBValue);
    
    char msg[64];
    snprintf(msg, 63, "Vals: %d %d", mChannelAValue, mChannelBValue);
    Serial.println(msg);  
}

void SpoolValve::Dither()
{
  if (mChannelAValue > 0)
  {
    analogWrite(mChannelAPin, constrain(mChannelAValue + random(-3,3), 0, 255));
  }
  else if (mChannelBValue > 0)
  {
    analogWrite(mChannelAPin, constrain(mChannelBValue + random(-3,3), 0, 255));    
  }
  
}


