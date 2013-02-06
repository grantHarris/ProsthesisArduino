#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  
  public:
  
    MotorController(PID PIDControl, int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, float dt, float mInput, float mOutput, float mSetpoint);
    
    void Update();
    
    void Iterate();
    
    float GetPressure();
  
  private:
    int mSensorPin;
    int mMotorPin;
    int mSetpointPin;
    int mKpPin;
    int mKiPin;
    int mKdPin;
    float mPressure;
    float mdt;
    
    PID mPIDControl;
    float mInput;
    float mOutput;
    float mSetpoint;
};
#endif __MOTOR_CONTROLLER_H_
