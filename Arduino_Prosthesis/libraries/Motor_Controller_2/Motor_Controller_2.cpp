#include "Motor_Controller_2.h"

MotorController::MotorController(double tInput, double tOutput, double tSetpoint,
                                 double tP, double tI, double tD, int tDirection,
		                         int tPPin, int tIPin, int tDPin,
		                         int tInputPin, int tOutputPin)

	:PID_Controller(&mcInput, &mcOutput, &mcSetpoint, tP, tI, tD, tDirection)
{
    mcInput = tInput;
	mcOutput = tOutput;
	mcSetpoint = tSetpoint;
	mcP = tP;
	mcI = tI;
	mcD = tD;
	mcPPin = tPPin;
	mcIPin = tIPin;
	mcDPin = tDPin;
	mcInputPin = tInputPin;
	mcOutputPin = tOutputPin;
}

void MotorController::Initialize(int time)
{
  mcInput = GetPressure(mcInputPin);
  PID_Controller.SetMode(AUTOMATIC);
  PID_Controller.SetSampleTime(time);
}//end Initialize(int)

void MotorController::Iterate()
{
  mcInput = GetPressure(mcInputPin);
  mcP = analogRead(mcPPin)*PID_POT_SENSITIVITY;
  mcI = analogRead(mcIPin)*PID_POT_SENSITIVITY;
  mcD = analogRead(mcDPin)*PID_POT_SENSITIVITY;
  PID_Controller.SetTunings(mcP,mcI,mcD);
}//end Iterate()

bool MotorController::Calculate()
{
  PID_Controller.Compute();
  analogWrite(mcOutputPin,mcOutput);
}//end Calculate()

double MotorController::GetPressure(int pin)
{
  return (double) (analogRead(pin)*ANALOG_TO_VOLTAGE*PRESSURE_SENSITIVITY)-PRESSURE_INTERCEPT;
}//end GetPressure(int)

void MotorController::SetSetpoint(double newSet)
{
  mcSetpoint = newSet;
}//end SetSetpoint(double)

void MotorController::AddToSetpoint(double addition)
{
  mcSetpoint = mcSetpoint + addition;
}//end AddToSetpoint(double)

double MotorController::GetP(){return mcP;}
double MotorController::GetI(){return mcI;}
double MotorController::GetD(){return mcD;}
double MotorController::GetInput(){return mcInput;}
double MotorController::GetSetpoint(){return mcSetpoint;}
double MotorController::GetOutput(){return mcOutput;}