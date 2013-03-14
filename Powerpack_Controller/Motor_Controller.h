#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  
  public:
  
    MotorController(int ModePin, int MotorPin, int InputPressurePin, int LoadSensePressurePin, int TuningBoxPin, int KpPin, int KiPin, int KdPin, double Kp, double Ki, double Kd, double dt, double LoadSenseOffset, double NumericalSetpoint, double Input, double Output, double Setpoint);
 
    void Initialize();
    
    void Iterate();
    
    double GetPressure(int Pin);
    
    void SetLoadSenseOffset(double NewOffset);
    
    void SetNumericalSetpoint(double NewSetpoint);
    
  private:
    int mModePin;
    int mMotorPin;
    int mInputPressurePin;
    int mLoadSensePressurePin;
    int mTuningBoxPin;
    int mKpPin;
    int mKiPin;
    int mKdPin;
    double mKp;
    double mKi;
    double mKd;
    double mPressure;
    double mdt;
    
    double mLoadSenseOffset;
    double mNumericalSetpoint;

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
