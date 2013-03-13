#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  
  public:
  
    MotorController(int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, double Kp, double Ki, double Kd, double dt, double mInput, double mOutput, double mSetpoint);
    
    void Initialize();
    
    void Iterate();
    
    double GetPressure();
    
  private:
    int mSensorPin;
    int mMotorPin;
    int mSetpointPin;
    int mKpPin;
    int mKiPin;
    int mKdPin;
    double mKp;
    double mKi;
    double mKd;
    double mPressure;
    double mdt;

    double mInput;
    double mOutput;
    double mSetpoint;
    
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
