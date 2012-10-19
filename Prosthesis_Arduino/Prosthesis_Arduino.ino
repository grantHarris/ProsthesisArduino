#include <MsTimer2.h>

#include "spool_valve.h"


const int kChannelAOutPin =  9;      // the number of the LED pin
const int kChannelBOutPin = 10;
const int kChannelAInputPin = 0;
const int kChannelBInputPin = 1;

SpoolValve LeftLeg = SpoolValve(kChannelAOutPin, kChannelBOutPin, kChannelAInputPin, kChannelBInputPin);

void setup() 
{
  Serial.begin(9600);
  MsTimer2::set(100, Dither10Hz);
  MsTimer2::start();
}

void Dither10Hz()
{
  LeftLeg.Dither();
}

void loop()
{
    LeftLeg.Update();
}

