/*
Basic Controlling Code
March 13, 2013
Thomas Watanabe
Acknowledgements: PID Library by Brett Beauregard
*/

#include <PID_v1.h>
#include <Motor_Controller.h>
#include <MsTimer2.h>
#include <Wire.h>

#define DISPLAY_ADDRESS1 0x70 //This is the default address of the OpenSegment with both solder jumpers open
#define DISPLAY_ADDRESS2 0x72

int LoadSensePlusPin = 23;
int LoadSenseMinusPin = 22;
int LoadSenseOffset = 500;

int NumericalSetpointPlusPin = 25;
int NumericalSetpointMinusPin = 24;
int NumericalSetpointPressure = 2500;

int ModePin = 10;
int DisplayPin = 11;

int InputPressurePin = 0;
int LoadSensePressurePin = 3;

int SegDelay = 0;
int DelayValue = 20;
double AverageInput = 0;
double AverageLoadSense = 0;

MotorController MyController(10,13,InputPressurePin,LoadSensePressurePin,4,5,6,7,0,0,0,10,500,2500,0,0,0);
/*
Controller Parameters:

Mode Pin = 10
Motor Pin = 13
Input Pressure Pin = 0
LoadSense Pressure Pin = 3
PID Tuning Box Pin = 4
Kp Pin = 5
Ki Pin = 6
Kd Pin = 7
Initial value of Kp = EEPROM value, 0 for now
Initial value of Ki = EEPROM value, 0 for now
Initial value of Kd = EEPROM value, 0 for now
Initial value of dt = 10ms
Initial value for LoadSense offset = 0 for now
Initial value for Numerical Setpoint = EEPROM value, 2500 for now
Initial value for Input = 0
Initial value for Setpoint = 0
Initial value for Output = 0
*/


void setup() {
  //for pid tune without final pid box, pull up pin 4
  pinMode(4,INPUT);
  digitalWrite(4,HIGH);
  //
  
  Wire.begin();
  Serial.begin(9600);

  Serial.println("PID Code");
  Wire.beginTransmission(DISPLAY_ADDRESS1);
  Wire.write('v');
  Wire.endTransmission();
  Wire.beginTransmission(DISPLAY_ADDRESS2);
  Wire.write('v');
  Wire.endTransmission();

  delay(2000);
  
  MyController.Initialize();
  MsTimer2::set(100, Control_10Hz);
  MsTimer2::start();
}

void Control_10Hz()
{
  MyController.Iterate();
}

void loop() 
{
  MsTimer2::stop();
  SegDelay++;
  AverageInput += MyController.GetPressure(InputPressurePin);
  AverageLoadSense += MyController.GetPressure(LoadSensePressurePin);
  if(digitalRead(DisplayPin) == HIGH)
  {
    if(SegDelay == DelayValue)
    {
      Wire.beginTransmission(DISPLAY_ADDRESS1);
      Wire.write(0x79);
      Wire.write(0x00);
      Wire.endTransmission();
      Wire.beginTransmission(DISPLAY_ADDRESS2);
      Wire.write(0x79);
      Wire.write(0x00);
      Wire.endTransmission();
      WriteValue(DISPLAY_ADDRESS1,AverageInput/SegDelay);
      WriteValue(DISPLAY_ADDRESS2,AverageLoadSense/SegDelay);
    }
  }
  else
  {
    WriteValue(DISPLAY_ADDRESS1,NumericalSetpointPressure);
    WriteValue(DISPLAY_ADDRESS2,LoadSenseOffset);
  }
  if(SegDelay == DelayValue)
  {
    SegDelay = 0;
    AverageInput = 0;
    AverageLoadSense = 0;
  }
  //Serial.print("Input Pressure: "); Serial.println(MyController.GetPressure(InputPressurePin));
  //Serial.print("LoadSense Pressure: "); Serial.println(MyController.GetPressure(LoadSensePressurePin));
  MsTimer2::start();
  
  // +/- Load Sense Offset
  if( digitalRead(LoadSensePlusPin) == HIGH )
  {
    LoadSenseOffset += 10;
    MyController.SetLoadSenseOffset(LoadSenseOffset);
    while( digitalRead(LoadSensePlusPin) == HIGH ) {} //sit in empty loop until button is un-pressed or 10Hz iteration interrupts
  }
  if( digitalRead(LoadSenseMinusPin) == HIGH )
  {
    LoadSenseOffset -= 10;
    MyController.SetLoadSenseOffset(LoadSenseOffset);
    while( digitalRead(LoadSenseMinusPin) == HIGH) {}
  }
  
  // +/- Numerical Setpoint
  if( digitalRead(NumericalSetpointPlusPin) == HIGH )
  {
    NumericalSetpointPressure += 50;
    MyController.SetNumericalSetpoint(NumericalSetpointPressure);
    while( digitalRead(NumericalSetpointPlusPin) == HIGH ) {}
  }
  if( digitalRead(NumericalSetpointMinusPin) == HIGH )
  {
    NumericalSetpointPressure -= 50;
    MyController.SetNumericalSetpoint(NumericalSetpointPressure);
    while( digitalRead(NumericalSetpointMinusPin) == HIGH ) {}
  }
}

void WriteValue(int address, double value)
{
  int truncatedValue = (int)value;
  /*if(value>=10000)
  {
    Wire.beginTransmission(address);
    Wire.write(0x77);//control display dots
    Wire.write(0xFF);//turn all on
    Wire.endTransmission();
  }
  else
  {*/
    Wire.beginTransmission(address);
    Wire.write(truncatedValue/1000);
    truncatedValue %= 1000;
    Wire.write(truncatedValue/100);
    truncatedValue %= 100;
    Wire.write(truncatedValue/10);
    truncatedValue %= 10;
    Wire.write(truncatedValue);
    Wire.endTransmission();
  //}
}
