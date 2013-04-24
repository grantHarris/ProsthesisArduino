#include <MsTimer2.h>
#include <PID_v1.h>
#include <Motor_Controller_2.h>

#define INPUT_PRESSURE_PIN 0
#define P_PIN 5
#define I_PIN 6
#define D_PIN 7
#define PWM_PIN 13
#define SETPOINT_PLUS_PIN 25
#define SETPOINT_MINUS_PIN 24

#define SYSTEM_PRINT_TIME 1000 //ms
#define SAMPLE_TIME 10 //ms


double pidInput = 0;
double pidSetpoint = 0;
double pidOutput = 0;
double pidP = 2;
double pidI = 5;
double pidD = 1;

boolean changeSetpointFlag = false;//if switch is pressed

unsigned long printTimer = 0;
boolean serialPrintFlag = false;

MotorController MController(pidInput, pidOutput, pidSetpoint, pidP, pidI, pidD, DIRECT, P_PIN, I_PIN, D_PIN, INPUT_PRESSURE_PIN, PWM_PIN);

void setup()
{
  Serial.begin(9600);
  
  MController.SetSetpoint(2500);
  MController.Initialize(SAMPLE_TIME);
  MsTimer2::set(1,TimedCommands);
  MsTimer2::start();
}//end setup()

void TimedCommands()
{
  MController.Calculate();
  if( (millis()-printTimer) > SYSTEM_PRINT_TIME)
  {
    printTimer = millis();
    serialPrintFlag = true;
  }
}//end TimedCommands()

void loop()
{
  MController.Iterate();
  Interface();
  if (serialPrintFlag == true)
  {
    Print_Info();
    serialPrintFlag = false;
    
  }
}//end loop()

void Print_Info()
{
  Serial.print("P:");Serial.println(MController.GetP());
  Serial.print("I:");Serial.println(MController.GetI());
  Serial.print("D:");Serial.println(MController.GetD());
  Serial.print("In:");Serial.println(MController.GetInput());
  Serial.print("Set:");Serial.println(MController.GetSetpoint());
  Serial.print("Out:");Serial.println(MController.GetOutput());
}//end Print_Info()

void Interface()
{
  //for setpoint switches
  if( changeSetpointFlag == false && digitalRead(SETPOINT_PLUS_PIN) == HIGH )
  {
    MController.AddToSetpoint(50);//pidSetpoint += 50;
    changeSetpointFlag = true;
  }
  else if( changeSetpointFlag == false && digitalRead(SETPOINT_MINUS_PIN) == HIGH )
  {
    MController.AddToSetpoint(-50);//pidSetpoint -= 50;
    changeSetpointFlag = true;
  }
  else if(digitalRead(SETPOINT_PLUS_PIN) == LOW && digitalRead(SETPOINT_MINUS_PIN) == LOW)
  {
    changeSetpointFlag = false;
  }
}//end Interface()

/*
double GetPressure(int Pin)
{
  return (double) (analogRead(Pin)*ANALOG_TO_VOLTAGE*PRESSURE_SENSITIVITY)-PRESSURE_INTERCEPT;
}//end GetPressure(int)

void Calculate()
{
  PID_Controller.Compute();
  analogWrite(PWM_PIN,pidOutput);
}//end Calculate()

void Iterate()
{  
  pidInput = GetPressure(INPUT_PRESSURE_PIN);
  pidP = analogRead(P_PIN)*PID_POT_SENSITIVITY;
  pidI = analogRead(I_PIN)*PID_POT_SENSITIVITY;
  pidD = analogRead(D_PIN)*PID_POT_SENSITIVITY;
  PID_Controller.SetTunings(pidP,pidI,pidD);
  
  
}//end Iterate()



void Initialize()
{
  pidInput = GetPressure(INPUT_PRESSURE_PIN);
  PID_Controller.SetMode(AUTOMATIC);
  PID_Controller.SetSampleTime(SAMPLE_TIME);
}
*/

