#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  
  public:
  
    MotorController(PID PIDControl, int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, float dt);
    
    void Iterate();
    
    
  
  private:
    PID mPIDControl;
    int mSensorPin;
    int mMotorPin;
    int mSetpointPin;
    int mKpPin;
    int mKiPin;
    int mKdPin;
    float mdt;
};
#endif __MOTOR_CONTROLLER_H_
