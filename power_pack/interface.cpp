#include <Arduino.h>
#include <Wire.h>
#include "interface.h"
#include "motors.h"
#include "pins.h"

#define DISPLAY_OUTPUT_PRESSURE 0x70 //This is the default address of the OpenSegment with both solder jumpers open
#define DISPLAY_PRESSURE_SETPOINT 0x72
#define CLEAR_DISPLAY 0x76 // Command to clear the 7seg display (sparkfun version)

namespace Interface
{
  void OpenDisplayConnections()
  {
    Wire.begin();
    Wire.beginTransmission(DISPLAY_OUTPUT_PRESSURE);
    Wire.write(CLEAR_DISPLAY);
    Wire.endTransmission();
    Wire.beginTransmission(DISPLAY_PRESSURE_SETPOINT);
    Wire.write(CLEAR_DISPLAY);
    Wire.endTransmission();
  }

  void WriteValueToDisplay(int address, int value)
  {
    Wire.beginTransmission(address);
    Wire.write(0x79);
    Wire.write(0x00);
    Wire.write(value/1000);
    value %= 1000;
    Wire.write(value/100);
    value %= 100;
    Wire.write(value/10);
    value %= 10;
    Wire.write(value);
    Wire.endTransmission();
  }

  void UpdateDisplays(float *getOutputPressure(), float *getPressureSetpoint())
  {
    WriteValueToDisplay(DISPLAY_OUTPUT_PRESSURE,(int) getOutputPressure());
    WriteValueToDisplay(DISPLAY_PRESSURE_SETPOINT,(int) getPressureSetpoint());
  }

  void UpdatePressureSetpoint(float *changeSetpoint(float))
  {
    if( mPressureSetpointPressed == false && digitalRead(SETPOINT_PLUS_PIN) == HIGH )
    {
      changeSetpoint(mSetpointIncr);
      mPressureSetpointPressed = true;
    }
    else if( mPressureSetpointPressed == false && digitalRead(SETPOINT_MINUS_PIN) == HIGH )
    {
      changeSetpoint(-mSetpointIncr);
      mPressureSetpointPressed = true;
    }
    else if(digitalRead(SETPOINT_PLUS_PIN) == LOW && digitalRead(SETPOINT_MINUS_PIN) == LOW)
    {
      mPressureSetpointPressed = false;
    }
  }

}
