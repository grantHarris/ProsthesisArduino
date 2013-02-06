#include "Motor_Controller.h"

MotorController::MotorController(int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, float Kp, float Ki, float Kd, float dt, float Input, float Output, float Setpoint)
:
mSensorPin(SensorPin),
mMotorPin(MotorPin),
mSetpointPin(SetpointPin),
mKpPin(KpPin),
mKiPin(KiPin),
mKdPin(KdPin),
mKp(Kp),
mKi(Ki),
mKd(Kd),
mdt(dt), 
mInput(Input),
mSetpoint(Setpoint),
mOutput(Output){}

void MotorController::Iterate()
{
  mKp = analogRead(mKpPin)/50.0;
  mKi = analogRead(mKiPin)/50.0;
  mKd = analogRead(mKdPin)/50.0;
  mPIDControl.SetTunings(mKp, mKi, mKd);
  
  mInput = GetPressure();
  mSetpoint = analogRead(mSetpointPin)*4.8875/2.0;
  
  mPIDControl.Compute();
  analogWrite(mMotorPin, mOutput);
}

float MotorController::GetPressure()
{
  int volt_sum = 0;
  for(int i=0, i<20, i++){
    volt_sum = volt_sum + analogRead(mSensorPin);
  }
  
  return (volt_sum/20.0)*(5.0/1023.0)*2*1250-1230;
}
