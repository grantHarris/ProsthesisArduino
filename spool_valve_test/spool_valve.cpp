#include "spool_valve.h"

const int inputDeadZone = 190;

SpoolValve::SpoolValve(int channelAPin, int channelBPin, int aInputPin, int bInputPin, int ditherRange)
: 
mChannelAPin(channelAPin),
mChannelBPin(channelBPin),
mAInputPin(aInputPin),
mBInputPin(bInputPin),
mDitherValue(0),
mDitherRange(ditherRange) {}

void SpoolValve::Update()
{
    // set the LED with the ledState of the variable:
    mChannelAValue = analogRead(mAInputPin);
    mChannelBValue = analogRead(mBInputPin);  
    
    if (mChannelAValue <= inputDeadZone)
    {
      mChannelAValue = 0;
      mChannelBValue -= inputDeadZone;
      mChannelBValue = constrain(((float)255 * (float)mChannelBValue / ((float)1024 - (float)inputDeadZone))+ mDitherValue, 0, 255);
    }
    else if (mChannelBValue <= inputDeadZone)
    {
      mChannelBValue = 0; 
      mChannelAValue -= inputDeadZone;
      mChannelAValue = constrain(((float)255 * (float)mChannelAValue / ((float)1024 - (float)inputDeadZone)) + mDitherValue, 0, 255);
    }
    
    analogWrite(mChannelAPin, mChannelAValue);
    analogWrite(mChannelBPin, mChannelBValue);
    
    char msg[64];
    snprintf(msg, 63, "Vals: %d %d", mChannelAValue, mChannelBValue);
    Serial.println(msg);  
}

void SpoolValve::Dither()
{
  mDitherValue = random(-mDitherRange, mDitherRange);
}


