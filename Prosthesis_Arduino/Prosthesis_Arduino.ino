#include <MsTimer2.h>

#include "GlobalConstants.h"
#include "spool_valve.h"

//Pin constants
const int kChannelAOutPin =  9; 
const int kChannelBOutPin = 10;
const int kChannelAInputPin = 0;
const int kChannelBInputPin = 1;

const int kDitherRange = 3;

SpoolValve LeftLeg = SpoolValve(kChannelAOutPin, kChannelBOutPin, kChannelAInputPin, kChannelBInputPin, kDitherRange);

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


