#ifndef __PROSTHESIS_MOTORS_H__
#define __PROSTHESIS_MOTORS_H__

namespace ProsthesisMotors
{
  void Initialize();
  void SetKneeMotorPinout(int kneeThrottleOut, int kneePressureIn, int kneeLoadIn);
  void SetHipMotorPinout(int hitThrottleOut, int hipPressureIn, int hidLoadIn);
  
  void ToggleKneeMotorControl(bool toggle);
  void ToggleHipMotorControl(bool toggle);
  
  void UpdateMotors();
}

#endif //__PROSTHESIS_MOTORS_H__

