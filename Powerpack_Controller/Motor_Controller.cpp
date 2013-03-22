#include "Motor_Controller.h"

MotorController::MotorController(int ModePin, int MotorPin, int InputPressurePin, int LoadSensePressurePin, int TuningBoxPin, int KpPin, int KiPin, int KdPin, double Kp, double Ki, double Kd, double dt, double LoadSenseOffset, double NumericalSetpoint, double mInput, double mOutput, double mSetpoint)
: mPIDControl(&mInput, &mOutput, &mSetpoint, Kp, Ki, Kd, DIRECT),
mModePin(ModePin),
mMotorPin(MotorPin),
mInputPressurePin(InputPressurePin),
mLoadSensePressurePin(LoadSensePressurePin),
mTuningBoxPin(TuningBoxPin),
mKpPin(KpPin),
mKiPin(KiPin),
mKdPin(KdPin),
mKp(Kp),
mKi(Ki),
mKd(Kd),
mdt(dt),
mLoadSenseOffset(LoadSenseOffset),
mNumericalSetpoint(NumericalSetpoint){}/*,
mInput(Input),
mSetpoint(Setpoint),
mOutput(Output){}*/

const double MotorController::PID_Pot_Sensitivity = .02;
const double MotorController::Setpoint_Pot_Sensitivity = 2.44375;
const double MotorController::Pressure_Sample_Count = 20.0;
const double MotorController::Pressure_Sensitivity = 2500.0;
const double MotorController::Pressure_Intercept = 1230.0;
const double MotorController::Analog_to_Voltage = 0.004887586;
const double MotorController::LoadSense_Test_Pressure_Max = 3000;
const double MotorController::SetPoint_Test_Pressure_Max = 3200;

void MotorController::Initialize()
{
    mPIDControl.SetMode(AUTOMATIC);
    mPIDControl.SetSampleTime(mdt);
    mPIDControl.SetTunings(mKp, mKi, mKd);
}

void MotorController::Iterate()
{
  if(digitalRead(mTuningBoxPin) == HIGH)
  {
    mKp = analogRead(mKpPin)*PID_Pot_Sensitivity;
    mKi = analogRead(mKiPin)*PID_Pot_Sensitivity;
    mKd = analogRead(mKdPin)*PID_Pot_Sensitivity;
    mPIDControl.SetTunings(mKp, mKi, mKd);
  }
  
  if(digitalRead(mModePin) == HIGH)
  {
    mSetpoint = GetPressure(mLoadSensePressurePin) + mLoadSenseOffset;
	if(mSetpoint > LoadSense_Test_Pressure_Max)
		mSetpoint = LoadSense_Test_Pressure_Max;
  }
  if(digitalRead(mModePin) == LOW)
  {
    mSetpoint = mNumericalSetpoint;
	if(mSetpoint > SetPoint_Test_Pressure_Max)
		mSetpoint = SetPoint_Test_Pressure_Max;
  }

  mInput = GetPressure(mInputPressurePin);
  mPIDControl.Compute();
  analogWrite(mMotorPin, mOutput);
}

double MotorController::GetPressure(int Pin)
{
  int volt_sum = 0;
  for(int i=0; i<int(Pressure_Sample_Count); i++){
    volt_sum = volt_sum + analogRead(Pin);
  }
  
  return (volt_sum/Pressure_Sample_Count)*Pressure_Sensitivity*Analog_to_Voltage-Pressure_Intercept;
}

void MotorController::SetLoadSenseOffset(double NewOffset)
{
  mLoadSenseOffset = NewOffset;
}

void MotorController::SetNumericalSetpoint(double NewSetpoint)
{
  mNumericalSetpoint = NewSetpoint;
}
