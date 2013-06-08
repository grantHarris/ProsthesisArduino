#ifndef __PROSTHESIS_MOTORS_H__
#define __PROSTHESIS_MOTORS_H__

#include <PID_v1.h>

#define PID_COMPUTE_PERIOD_MS 25
#define HIP_MOTOR_INDEX 0
#define KNEE_MOTOR_INDEX 1

namespace ProsthesisMotors
{
  typedef struct tMotorConfig {
    tMotorConfig() : 
    mThrottlePin(-1), 
    mPressureInputPin(-1), 
    mLoadInputPin(-1), 
    mActive(false), 
    mSampleAvg(0.0f),
    mP(0.0f), 
    mI(0.0f), 
    mD(0.0f), 
    mPressureSetpoint(1500),
    mThrottle(0),
    mPIDController(&mSampleAvg, &mThrottle, &mPressureSetpoint, 0, 0, 0, DIRECT)
    {
      mPIDController.SetMode(AUTOMATIC);
      mPIDController.SetSampleTime(PID_COMPUTE_PERIOD_MS);
    }
   
    uint8_t mThrottlePin;
    uint8_t mPressureInputPin;
    uint8_t mLoadInputPin;
    double mSampleAvg;
    float mP;
    float mI;
    float mD;
    double mPressureSetpoint;
    double mThrottle;
    bool mActive;
    
    PID mPIDController;
  } tMotorConfig;  
  
  void Initialize();
  void SetKneeMotorPinout(int kneeThrottleOut, int kneePressureIn, int kneeLoadIn);
  void SetHipMotorPinout(int hitThrottleOut, int hipPressureIn, int hidLoadIn);
  
  void ToggleKneeMotorControl(bool toggle);
  void ToggleHipMotorControl(bool toggle);
  
  void UpdateMotors();
  
  const tMotorConfig *GetHipMotorConfig();
  void ChangeHipMotorSetPoint(int amount);
  const tMotorConfig *GetKneeMotorConfig();
  void ChangeKneeMotorSetPoint(int amount);
}

#endif //__PROSTHESIS_MOTORS_H__

