#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  
  public:
  
    MotorController(int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, float Kp, float Ki, float Kd, float dt, float mInput, float mOutput, float mSetpoint);
    
    void Initialize();
    
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
    
    PID mPIDControl;
    
    //conversion constants
    static const double PID_Pot_Sensitivity;
    static const double Setpoint_Pot_Sensitivity;
    static const double Pressure_Sample_Count; //double instead of int to avoid integer division
    static const double Pressure_Sensitivity;
    static const double Pressure_Intercept;
    static const double Analog_to_Voltage;

};
#endif __MOTOR_CONTROLLER_H_
