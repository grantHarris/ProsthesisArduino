#include <Arduino.h>
#include <PID_v1.h>
#include <pot_box.h>
#include "motors.h"
#include "pins.h"

void setup()
{
  Serial.begin(9600);
  ProsthesisMotors::Initialize();
  ProsthesisMotors::SetKneeMotorPinout(MOTOR_KNEE_THROTTLE_PIN, MOTOR_KNEE_PRESSURE_IN, MOTOR_KNEE_LOAD_IN);
  ProsthesisMotors::SetHipMotorPinout(MOTOR_HIP_THROTTLE_PIN, MOTOR_HIP_PRESSURE_IN, MOTOR_HIP_LOAD_IN);
  
  ProsthesisMotors::ToggleKneeMotorControl(false);
  ProsthesisMotors::ToggleHipMotorControl(false);
}

void loop()
{
  ProsthesisMotors::UpdateMotors();
}
