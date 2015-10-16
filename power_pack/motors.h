#ifndef __PROSTHESIS_MOTORS_H__
#define __PROSTHESIS_MOTORS_H__

#include <PID_v1.h>

#define PID_COMPUTE_PERIOD_MS 25
#define HIP_MOTOR_INDEX 0
#define KNEE_MOTOR_INDEX 1

#define ANALOG_TO_VOLTAGE 0.004894045
#define PRESSURE_SENSITIVITY 1338.104717
#define PRESSURE_INTERCEPT 1246.725105

#define ANALOG_READ_TO_PRESSURE(x) (((double)x * ANALOG_TO_VOLTAGE * PRESSURE_SENSITIVITY) - PRESSURE_INTERCEPT)

namespace ProsthesisMotors
{
  typedef struct tMotorConfig {
    tMotorConfig(int minThrottle) : 
    mThrottlePin(-1), 
    mPressureInputPin(-1), 
    mLoadInputPin(-1), 
    mActive(false), 
    mSampleAvg(0.0f),
    mP(0.0f), 
    mI(0.0f), 
    mD(0.0f), 
    mPressureSetpoint(1500),
    mMinPressureSetpoint(0),
    mMaxPressureSetpoint(3500),
    mThrottle(0),
    mPIDController(&mSampleAvg, &mThrottle, &mPressureSetpoint, 0, 0, 0, DIRECT),
    mMinThrottle(minThrottle)
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
    double mMinPressureSetpoint;
    double mMaxPressureSetpoint;
    double mThrottle;
    bool mActive;
    int mMinThrottle;
    
    PID mPIDController;
    
    void Update()
    {
      if (mActive)
      {
        //Use exponential averaging from http://bleaklow.com/2012/06/20/sensor_smoothing_and_optimised_maths_on_the_arduino.html
        mSampleAvg = 0.1 * ANALOG_READ_TO_PRESSURE(analogRead(mPressureInputPin)) + 0.9 * mSampleAvg;
        if (mPIDController.Compute())
        {
          mThrottle = mThrottle > mMinThrottle ? mThrottle : 0;
          analogWrite(mThrottlePin, mThrottle);
        }
      }    
    }
  } tMotorConfig;  
  
  void Initialize();
  void SetKneeMotorPinout(int kneeThrottleOut, int kneePressureIn, int kneeLoadIn);
  void SetHipMotorPinout(int hitThrottleOut, int hipPressureIn, int hidLoadIn);
  
  void ToggleKneeMotorControl(bool toggle);
  void ToggleHipMotorControl(bool toggle);
  
  void UpdateMotors();
  
  const tMotorConfig *GetHipMotorConfig();
  const tMotorConfig *GetKneeMotorConfig();

  int GetMinPressureSetPoint();
  int GetMaxPressureSetPoint();

  void ChangeMinMotorSetPoint(int amount);
  void ChangeMaxMotorSetPoint(int amount);

}

#endif //__PROSTHESIS_MOTORS_H__


