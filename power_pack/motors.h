#ifndef __PROSTHESIS_MOTORS_H__
#define __PROSTHESIS_MOTORS_H__

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
    tMotorConfig(int minThrottle, int maxThrottle, double scale) : 
    mThrottlePin(-1), 
    mPressureInputPin(-1), 
    mLoadInputPin(-1), 
    mActive(false), 
    mSampleAvg(0.0f),
    mP(0.0f), 
    mI(0.0f), 
    mD(0.0f), 
    mMinPressureSetpoint(0),
    mMaxPressureSetpoint(3500),
    mThrottle(0),
    mMinThrottle(minThrottle),
    mMaxThrottle(maxThrottle),
    mScale(scale){
    }  

    uint8_t mThrottlePin;
    uint8_t mPressureInputPin;
    uint8_t mLoadInputPin;
    double mSampleAvg;
    float mP;
    float mI;
    float mD;
    double mMinPressureSetpoint;
    double mMaxPressureSetpoint;
    double mThrottle;
    bool mActive;
    int mMinThrottle;
    int mMaxThrottle;
    float mScale;

    void Update()
    {
      if (mActive)
      {
        //Use exponential averaging from http://bleaklow.com/2012/06/20/sensor_smoothing_and_optimised_maths_on_the_arduino.html
        mSampleAvg = ANALOG_READ_TO_PRESSURE(analogRead(mPressureInputPin));
        

//        Serial.print("Pressure sensor");
//        Serial.println(mSampleAvg, DEC);
//        Serial.print("mMaxPressureSetpoint: ");
//        Serial.println(mMaxPressureSetpoint, DEC);
//
//        Serial.print("mMinPressureSetpoint: ");
//        Serial.println(mMinPressureSetpoint, DEC);
        
        mThrottle = (  (mMaxPressureSetpoint - mSampleAvg)  /  (mMaxPressureSetpoint - mMinPressureSetpoint)  ) / 255;
        
        mThrottle = (mSampleAvg < mMinPressureSetpoint) ? 255 : mThrottle;
        mThrottle = (mSampleAvg > mMaxPressureSetpoint) ? 0 : mThrottle;

//        Serial.print("Motor value");
//        Serial.println(mThrottle * mScale, DEC);
//        Serial.println();
//        Serial.println();
        analogWrite(mThrottlePin, mThrottle * mScale);

      }    
    }
  } tMotorConfig;  
  
  void Initialize(int minPressureSetpoint, int maxPressureSetpoint);
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


