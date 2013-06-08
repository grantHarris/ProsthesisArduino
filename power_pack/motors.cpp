//#define DEBUG_POT_BOX 1
//#define DEBUG_KNEE_MOTOR 1
//#define DEBUG_HIP_MOTOR 1

#include <Arduino.h>
#include <pot_box.h>
#include "motors.h"
#include "pins.h"

#if DEBUG_POT_BOX || DEBUG_KNEE_MOTOR || DEBUG_HIP_MOTOR
//Handy wrapper for using stream-like serial printing
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
#endif

//Constant terms
#define PID_POT_SENSITIVITY 0.02
#define ANALOG_TO_VOLTAGE 0.004892494
#define PRESSURE_SENSITIVITY 1386.2
#define PRESSURE_INTERCEPT 1246.7

#define SAFE_MAX_PRESSURE_PSI 2500
#define SAFE_MIN_PRESSURE_PSI 0

#define ANALOG_READ_TO_PRESSURE(x) (((double)x * ANALOG_TO_VOLTAGE * PRESSURE_SENSITIVITY) - PRESSURE_INTERCEPT)

namespace ProsthesisMotors
{
  tMotorConfig mKneeMotorConfig;
  tMotorConfig mHipMotorConfig;
  
  int mPotBoxConnected = false;
  int mPotBoxConnectionDirty = false;
  
  void PotBoxConnectionDirtyCB()
  {
    mPotBoxConnectionDirty = true;
  }
  
  void Initialize()
  {
    ProsthesisPotBox::Initialize(POT_BOX_INTERRUPT_ID, POT_BOX_INTERRUPT_PIN, POT_BOX_P_PIN, POT_BOX_I_PIN, POT_BOX_D_PIN, 0, 0, 0);
    mPotBoxConnected = ProsthesisPotBox::IsConnected();
    ProsthesisPotBox::SetConnectionDirtyCallback(PotBoxConnectionDirtyCB);
  }
  
  void SetKneeMotorPinout(int kneeThrottleOut, int kneePressureIn, int kneeLoadIn)
  {
    mKneeMotorConfig.mThrottlePin = kneeThrottleOut;
    mKneeMotorConfig.mPressureInputPin = kneePressureIn;
    mKneeMotorConfig.mLoadInputPin = kneeLoadIn;
  }
  
  void SetHipMotorPinout(int hitThrottleOut, int hipPressureIn, int hidLoadIn)
  {
    mHipMotorConfig.mThrottlePin = hitThrottleOut;
    mHipMotorConfig.mPressureInputPin = hipPressureIn;
    mHipMotorConfig.mLoadInputPin = hidLoadIn;    
  }
  
  void ToggleKneeMotorControl(bool toggle)
  {
    mKneeMotorConfig.mActive = toggle;
    mKneeMotorConfig.mThrottle = 0;
    analogWrite(mKneeMotorConfig.mThrottlePin, mKneeMotorConfig.mThrottle);
  }
  
  void ToggleHipMotorControl(bool toggle)
  {
    mHipMotorConfig.mActive = toggle;
    mHipMotorConfig.mThrottle = 0;
    analogWrite(mHipMotorConfig.mThrottlePin, mHipMotorConfig.mThrottle);
  }
 
  void UpdateMotors()
  { 
    if (mPotBoxConnectionDirty || !mPotBoxConnected)
    {
      ProsthesisPotBox::AttemptReconnect(&mPotBoxConnected);
#if DEBUG_POT_BOX
      const char *connectMsg = mPotBoxConnected ? "connected" : "disconnected";
      Serial << "PID box is now " << connectMsg << "\n";
#endif
      
      mPotBoxConnectionDirty = false;
    }
    
#if DEBUG_POT_BOX || DEBUG_KNEE_MOTOR || DEBUG_HIP_MOTOR
    delay(50);
#endif    
    
    if (mPotBoxConnected)
    {
      int intP;
      int intI;
      int intD;
      ProsthesisPotBox::GetPID(&intP, &intI, &intD);
      mKneeMotorConfig.mP = intP * PID_POT_SENSITIVITY;
      mKneeMotorConfig.mI = intI * PID_POT_SENSITIVITY;
      mKneeMotorConfig.mD = intD * PID_POT_SENSITIVITY;
      mKneeMotorConfig.mPIDController.SetTunings(mKneeMotorConfig.mP, mKneeMotorConfig.mI, mKneeMotorConfig.mD);
      
      mHipMotorConfig.mP = mKneeMotorConfig.mP;
      mHipMotorConfig.mI = mKneeMotorConfig.mI;
      mHipMotorConfig.mD = mKneeMotorConfig.mD;
      mHipMotorConfig.mPIDController.SetTunings(mHipMotorConfig.mP, mHipMotorConfig.mI, mHipMotorConfig.mD);      
      
#if DEBUG_POT_BOX
      Serial << "Read pot box " << intP << " " << intI << " " << intD << "\n";
#endif      
    }
    
    if (mKneeMotorConfig.mActive)
    {
      //Use exponential averaging from http://bleaklow.com/2012/06/20/sensor_smoothing_and_optimised_maths_on_the_arduino.html
      mKneeMotorConfig.mSampleAvg = 0.1 * ANALOG_READ_TO_PRESSURE(analogRead(mKneeMotorConfig.mPressureInputPin)) + 0.9 * mKneeMotorConfig.mSampleAvg;
      if (mKneeMotorConfig.mPIDController.Compute())
      {
#if DEBUG_KNEE_MOTOR
          Serial << "K: T: " << (int)mKneeMotorConfig.mThrottle << ". P: " << mKneeMotorConfig.mSampleAvg << " PID: " << mKneeMotorConfig.mP << ", " << mKneeMotorConfig.mI << ", " << mKneeMotorConfig.mD << "\n";
#endif
        analogWrite(mKneeMotorConfig.mThrottlePin, mKneeMotorConfig.mThrottle);
      }
    }
    
    if (mHipMotorConfig.mActive)
    {
      //Use exponential averaging from http://bleaklow.com/2012/06/20/sensor_smoothing_and_optimised_maths_on_the_arduino.html
      mHipMotorConfig.mSampleAvg = 0.1 * ANALOG_READ_TO_PRESSURE(analogRead(mHipMotorConfig.mPressureInputPin)) + 0.9 * mHipMotorConfig.mSampleAvg;
      if (mHipMotorConfig.mPIDController.Compute())
      {
#if DEBUG_HIP_MOTOR        
        Serial << "H: T: " << (int)mKneeMotorConfig.mThrottle << ". P: " << mKneeMotorConfig.mSampleAvg << " PID: " << mKneeMotorConfig.mP << ", " << mKneeMotorConfig.mI << ", " << mKneeMotorConfig.mD << "\n";        
#endif
        analogWrite(mHipMotorConfig.mThrottlePin, mHipMotorConfig.mThrottle);
      }
    }
  } 
  
  const tMotorConfig *GetHipMotorConfig()
  {
    return &mHipMotorConfig;  
  }
  
  void ChangeHipMotorSetPoint(int amount)
  {
    mHipMotorConfig.mPressureSetpoint = constrain(amount + mHipMotorConfig.mPressureSetpoint, SAFE_MIN_PRESSURE_PSI, SAFE_MAX_PRESSURE_PSI);
  }
  
  const tMotorConfig *GetKneeMotorConfig()
  {
    return &mKneeMotorConfig;
  }  
  
  void ChangeKneeMotorSetPoint(int amount)
  {
    mKneeMotorConfig.mPressureSetpoint = constrain(amount + mKneeMotorConfig.mPressureSetpoint, SAFE_MIN_PRESSURE_PSI, SAFE_MAX_PRESSURE_PSI);
  }
}

