/*
Basic Controlling Code
March 13, 2013
Thomas Watanabe
Acknowledgements: PID Library by Brett Beauregard
*/

#include <PID_v1.h>
#include <Motor_Controller.h>
#include <MsTimer2.h>

int LoadSensePlusPin = 21;
int LoadSenseMinusPin = 22;
int LoadSenseOffset = 500;

int NumericalSetpointPlusPin = 23;
int NumericalSetpointMinusPin = 24;
int NumericalSetpointPressure = 2500;

MotorController MyController(0,1,2,3,4,5,6,7,0,0,0,10,500,2500,0,0,0);
/*
Controller Parameters:

Mode Pin = 0
Motor Pin = 1
Input Pressure Pin = 2
LoadSense Pressure Pin = 3
PID Tuning Box Pin = 4
Kp Pin = 5
Ki Pin = 6
Kd Pin = 7
Initial value of Kp = EEPROM value, 0 for now
Initial value of Ki = EEPROM value, 0 for now
Initial value of Kd = EEPROM value, 0 for now
Initial value of dt = 10ms
Initial value for LoadSense offset = 0 for now
Initial value for Numerical Setpoint = EEPROM value, 2500 for now
Initial value for Input = 0
Initial value for Setpoint = 0
Initial value for Output = 0
*/


void setup() {
  Serial.begin(9600);

  Serial.println("PID Code");

  delay(2000);
  
  MyController.Initialize();
  MsTimer2::set(100, Control_10Hz);
  MsTimer2::start();
}

void Control_10Hz()
{
  MyController.Iterate();
}

void loop() 
{
  // +/- Load Sense Offset
  if( digitalRead(LoadSensePlusPin) == HIGH )
  {
    LoadSenseOffset += 10;
    MyController.SetLoadSenseOffset(LoadSenseOffset);
    while( digitalRead(LoadSensePlusPin) == HIGH ) {} //sit in empty loop until button is un-pressed or 10Hz iteration interrupts
  }
  if( digitalRead(LoadSenseMinusPin) == HIGH )
  {
    LoadSenseOffset -= 10;
    MyController.SetLoadSenseOffset(LoadSenseOffset);
    while( digitalRead(LoadSenseMinusPin) == HIGH) {}
  }
  
  // +/- Numerical Setpoint
  if( digitalRead(NumericalSetpointPlusPin) == HIGH )
  {
    NumericalSetpointPressure += 50;
    MyController.SetNumericalSetpoint(NumericalSetpointPressure);
    while( digitalRead(LoadSensePlusPin) == HIGH ) {}
  }
  if( digitalRead(NumericalSetpointMinusPin) == HIGH )
  {
    NumericalSetpointPressure -= 50;
    MyController.SetNumericalSetpoint(NumericalSetpointPressure);
    while( digitalRead(LoadSenseMinusPin) == HIGH ) {}
  }
}
