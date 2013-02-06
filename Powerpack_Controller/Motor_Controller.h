#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  
  public:
  
    MotorController(int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, float Kp, float Ki, float Kd, float dt, float mInput, float mOutput, float mSetpoint);
    
    void Iterate();
    
    float GetPressure();
    
  private:
    int mSensorPin;
    int mMotorPin;
    int mSetpointPin;
    int mKpPin;
    int mKiPin;
    int mKdPin;
    float mKp;
    float mKi;
    float mKd;
    float mPressure;
    float mdt;

    float mInput;
    float mOutput;
    float mSetpoint;
    
    PID mPIDControl(&mInput, &mOutput, &mSetpoint, mKp, mKi, mKd, DIRECT);

};
#endif __MOTOR_CONTROLLER_H_
