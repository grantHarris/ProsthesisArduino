#include "Motor_Controller.h"

MotorController::MotorController(int SensorPin, int MotorPin, int SetpointPin, int KpPin, int KiPin, int KdPin, float Kp, float Ki, float Kd, float dt, float Input, float Output, float Setpoint)
: mPIDControl(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT),
mSensorPin(SensorPin),
mMotorPin(MotorPin),
mSetpointPin(SetpointPin),
mKpPin(KpPin),
mKiPin(KiPin),
mKdPin(KdPin),
mKp(Kp),
mKi(Ki),
mKd(Kd),
mdt(dt), 
mInput(Input),
mSetpoint(Setpoint),
mOutput(Output){}

const MotorController::PID_Pot_Sensitivity = .02;
const MotorController::Setpoint_Pot_Sensitivity = 2.44375;
const MotorController::Pressure_Sample_Count = 20.0;
const MotorController::Pressure_Sensitivity = 2500.0;
const MotorController::Pressure_Intercept = 1230.0;
const MotorController::Analog_to_Voltage = 0.004887586;

void MotorController::Initialize()
{
    mPIDControl.SetMode(AUTOMATIC);
    mPIDControl.SetSampleTime(mdt);
}

void MotorController::Iterate()
{
  mKp = analogRead(mKpPin)*PID_Pot_Sensitivity;
  mKi = analogRead(mKiPin)*PID_Pot_Sensitivity;
  mKd = analogRead(mKdPin)*PID_Pot_Sensitivity;
  mPIDControl.SetTunings(mKp, mKi, mKd);
  
  mInput = GetPressure();
  mSetpoint = analogRead(mSetpointPin)*Setpoint_Pot_Sensitivity;
  
  mPIDControl.Compute();
  analogWrite(mMotorPin, mOutput);
}

float MotorController::GetPressure()
{
  int volt_sum = 0;
  for(int i=0; i<int(Pressure_Sample_Count); i++){
    volt_sum = volt_sum + analogRead(mSensorPin);
  }
  
  return (volt_sum/Pressure_Sample_Count)*Pressure_Sensitivity*Analog_to_Voltage-Pressure_Intercept;
}
