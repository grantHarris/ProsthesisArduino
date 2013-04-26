#ifndef __MOTOR_CONTROLLER_2_H_
#define __MOTOR_CONTROLLER_2_H_

#include <Arduino.h>
#include <PID_v1.h>

class MotorController
{
  public:
    
	//Constant terms
	#define PID_POT_SENSITIVITY 0.02
    #define ANALOG_TO_VOLTAGE 0.004887586
    #define PRESSURE_SENSITIVITY 2500
    #define PRESSURE_INTERCEPT 1230
	#define INPUT_AVG_COUNT 5
	
    MotorController(double tInput, double tOutput, double tSetpoint,
                                 double tP, double tI, double tD, int tDirection,
		                         int tPPin, int tIPin, int tDPin,
		                         int tInputPin, int tOutputPin);
								 
    void Initialize(int);
	
	void Iterate();
  
    bool Calculate();
  
    double GetPressure(int);
	
	void SetSetpoint(double);
	
	void AddToSetpoint(double);
	
	double GetP();
	double GetI();
	double GetD();
	double GetInput();
	double GetSetpoint();
	double GetOutput();
	

  private:
    
	PID PID_Controller;
	
    double mcInput;
	double mcOutput;
	double mcSetpoint;
	double mcP;
	double mcI;
	double mcD;
	int mcPPin;
	int mcIPin;
	int mcDPin;
	int mcInputPin;
	int mcOutputPin;
};
#endif __MOTOR_CONTROLLER_2_H_