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

#define SAFE_MAX_PRESSURE_PSI 2500
#define SAFE_MIN_PRESSURE_PSI 0

const float kMinThrottle = 0.02 * 255;

namespace ProsthesisMotors
{
  tMotorConfig mKneeMotorConfig(kMinThrottle);
  tMotorConfig mHipMotorConfig(kMinThrottle);
  
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
  
  void SetHipMotorPinout(int hipThrottleOut, int hipPressureIn, int hipLoadIn)
  {
    mHipMotorConfig.mThrottlePin = hipThrottleOut;
    mHipMotorConfig.mPressureInputPin = hipPressureIn;
    mHipMotorConfig.mLoadInputPin = hipLoadIn;    
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
      //GOTODO: Fix Integral parameter      
      mKneeMotorConfig.mI = 0;//intI * PID_POT_SENSITIVITY;
      mKneeMotorConfig.mD = intD * PID_POT_SENSITIVITY;
      mKneeMotorConfig.mPIDController.SetTunings(mKneeMotorConfig.mP, mKneeMotorConfig.mI, mKneeMotorConfig.mD);
      
      mHipMotorConfig.mP = mKneeMotorConfig.mP;
      //GOTODO: Fix Integral parameter
      mHipMotorConfig.mI = 0;//mKneeMotorConfig.mI;
      mHipMotorConfig.mD = mKneeMotorConfig.mD;
      mHipMotorConfig.mPIDController.SetTunings(mHipMotorConfig.mP, mHipMotorConfig.mI, mHipMotorConfig.mD);      
      
#if DEBUG_POT_BOX
      Serial << "Read pot box " << intP << " " << intI << " " << intD << "\n";
#endif      
    }
    
    mKneeMotorConfig.Update();
    
#if DEBUG_KNEE_MOTOR
    Serial << "K: T: " << (int)mKneeMotorConfig.mThrottle << ". P: " << mKneeMotorConfig.mSampleAvg << " PID: " << mKneeMotorConfig.mP << ", " << mKneeMotorConfig.mI << ", " << mKneeMotorConfig.mD << "\n";
#endif    
    
#if DEBUG_HIP_MOTOR        
     Serial << "H: T: " << (int)mHipMotorConfig.mThrottle << ". P: " << mHipMotorConfig.mSampleAvg << " PID: " << mHipMotorConfig.mP << ", " << mHipMotorConfig.mI << ", " << mHipMotorConfig.mD << "\n";        
#endif    

    mHipMotorConfig.Update();
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

