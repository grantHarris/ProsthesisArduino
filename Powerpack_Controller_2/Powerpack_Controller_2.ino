#include <MsTimer2.h>
#include <PID_v1.h>

#define INPUT_PRESSURE_PIN 0
#define P_PIN 5
#define I_PIN 6
#define D_PIN 7
#define PWM_PIN 13
#define SETPOINT_PLUS_PIN 25
#define SETPOINT_MINUS_PIN 24


#define SAMPLE_TIME 10 //ms

#define PID_POT_SENSITIVITY 0.02
#define ANALOG_TO_VOLTAGE 0.004887586
#define PRESSURE_SENSITIVITY 2500
#define PRESSURE_INTERCEPT 1230

double pidInput = 0;
double pidSetpoint = 0;
double pidOutput = 0;
double pidP = 2;
double pidI = 5;
double pidD = 1;

boolean changeSetpoint = false;

PID PID_Controller(&pidInput, &pidOutput, &pidSetpoint, pidP, pidI, pidD, DIRECT);

void setup()
{
  Serial.begin(9600);
  pidInput = GetPressure(INPUT_PRESSURE_PIN);
  pidSetpoint = 2500;
  
  PID_Controller.SetMode(AUTOMATIC);
  PID_Controller.SetSampleTime(SAMPLE_TIME);
  
  MsTimer2::set(1000,Print_Info);
  MsTimer2::start();
  
}

void Print_Info()
{
  Serial.print("P:");Serial.println(pidP);
  Serial.print("I:");Serial.println(pidI);
  Serial.print("D:");Serial.println(pidD);
  Serial.print("In:");Serial.println(pidInput);
  Serial.print("Set:");Serial.println(pidSetpoint);
  Serial.print("Out:");Serial.println(pidOutput);
}

void loop()
{
  pidInput = GetPressure(INPUT_PRESSURE_PIN);
  pidP = analogRead(P_PIN)*PID_POT_SENSITIVITY;
  pidI = analogRead(I_PIN)*PID_POT_SENSITIVITY;
  pidD = analogRead(D_PIN)*PID_POT_SENSITIVITY;
  PID_Controller.SetTunings(pidP,pidI,pidD);
  if( changeSetpoint == false && digitalRead(SETPOINT_PLUS_PIN) == HIGH )
  {
    pidSetpoint += 50;
    changeSetpoint = true;
  }
  else if( changeSetpoint == false && digitalRead(SETPOINT_MINUS_PIN) == HIGH )
  {
    pidSetpoint -= 50;
    changeSetpoint = true;
  }
  else if(digitalRead(SETPOINT_PLUS_PIN) == LOW && digitalRead(SETPOINT_MINUS_PIN) == LOW)
  {
    changeSetpoint = false;
  }
  
  PID_Controller.Compute();
  analogWrite(PWM_PIN,pidOutput);
  
}

double GetPressure(int Pin)
{
  
 
  return (double) (analogRead(Pin)*ANALOG_TO_VOLTAGE*PRESSURE_SENSITIVITY)-PRESSURE_INTERCEPT;
}

