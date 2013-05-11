#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#include <Arduino.h>
#include <PID_v1.h>
#include <pot_box.h>

class MotorController
{
  public:
    
	//Constant terms
	#define PID_POT_SENSITIVITY 0.02
    #define ANALOG_TO_VOLTAGE 0.004892494
    #define PRESSURE_SENSITIVITY 1386.2
    #define PRESSURE_INTERCEPT 1246.7
	#define INPUT_AVG_COUNT 5
	
    MotorController(double tInput, double tOutput, double tSetpoint,
                                 double tP, double tI, double tD, int tDirection,
		                         int tPPin, int tIPin, int tDPin,
		                         int tInputPin, int tOutputPin,
								 int tBoxInterruptPin, int tBoxConnectionPin);
								 
    void Initialize(int);
	
	void MarkConnectionDirty();
	
	void UpdateState(int);
  
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
	int GetBoxIsConnected();
	bool GetBoxConnectionDirty();

  private:
    
	PID PID_Controller;
	ProsthesisPotBox PID_PotBox;
	
    static double mcInput;
	static double mcOutput;
	static double mcSetpoint;
	double mcP;
	double mcI;
	double mcD;
	int mcInputArray[INPUT_AVG_COUNT];
	int mcInputTrack;
	int mcPPin;
	int mcIPin;
	int mcDPin;
	int mcInputPin;
	int mcOutputPin;
	int mcBoxInterruptID;
	int mcBoxConnectionPin;
	int mcBoxIsConnected;
	bool mcBoxConnectionDirty;
	static void WrapperForMarkConnectionDirty();
	static MotorController* sMotorCont;
};
#endif __MOTOR_CONTROLLER_H_