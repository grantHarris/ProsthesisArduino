/*
Basic Controlling Code
November 23 2012
Thomas Watanabe
Acknowledgements: PID Library by Brett Beauregard
*/

#include <PID_v1.h>

//global variables
//pins
int pbutton = 2;    //hook up pushbutton to digital pin 2
int targetpot = 2;  //hook up pressure controlling pin to analog pin 2
int kppot = 3;      //hook up potentiometer for kp (p const. for PID) to analog pin 3
int kipot = 4;      //hook up pot for ki to analog pin 4
int kdpot = 5;      //hook up pot for kd to analog pin 5
int motorPin = 10;   //hook up motor controller to digital pin 10
int sensorPin = 0;  //hook up pressure sensor output to analog pin 0
//working variables
boolean newpoint = false;
double phigh = 1500.0;
double plow = 0.0;
double pb = 1250.0; //y intercept if 0V mapped to 0psi
double vhigh;
double vlow;
double voltage;
double pressure;
double sensitivity;
double volt_sum;    //sample voltages many times, then average them
//PID stuff
double kp;
double ki;
double kd;
double Input;
double Output;
double Setpoint;
double presetpoint;
PID myPID(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);

void setup() {
  // Initialize the serial port
  Serial.begin(9600);
  
  // send an intro
  Serial.println("Welcome to the PID testing code.");
  delay(1000);
  
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  
  Serial.println("Set system to low pressure point");
  Serial.println("Push the button once this is done");
  //read voltage at low pressure 
  while(digitalRead(pbutton) == LOW) {
    vlow = analogRead(sensorPin);
  }
  //detect when low pressure reading is done
  if(digitalRead(pbutton) == HIGH){
    newpoint = true;
    delay(1000);
    Serial.println("Set system to high pressure point");
    Serial.println("Push the button once this is done");
  }
  
  //read voltage at high pressure 
  while(newpoint == true && digitalRead(pbutton) == LOW){
    vhigh = analogRead(sensorPin);
  }
  Serial.println("Calculating...");
  sensitivity = (phigh-plow)/(vhigh-vlow);
  delay(2000);
  Serial.println("Sensitivity: ");
  Serial.print(sensitivity*1023/5);
  Serial.println(" psi/V");
  Serial.println("Monitoring pressure...");
}

void loop() {
  //Read voltage across resistor
  volt_sum = 0.0;
  for(int i=0;i<20;i++){
    volt_sum = volt_sum + analogRead(sensorPin);
  }
  //average voltage reading
  voltage = volt_sum/20;
  //convert voltage to pressure
  pressure = voltage*sensitivity - pb;
  
  //here we set the PID values to the current values of each pot
  //thus, the PID constant values are updated every single loop()
  //if this is slow or unneeded, move this to the setup()
  kp = analogRead(kppot);
  ki = analogRead(kipot);
  kd = analogRead(kdpot); 
  myPID.SetTunings(kp, ki, kd);
  
  //do PID stuff
  presetpoint = analogRead(targetpot);
  if (presetpoint < vlow){
    Setpoint = vlow;
  }
  else
    Setpoint = presetpoint;
  Input = voltage;//analogRead(sensorPin);
  myPID.Compute();
  
  analogWrite(motorPin, Output);
  
  /*
  Serial.print("The voltage across the resistor is ");
  Serial.print(voltage*5/1023);
  Serial.println(" V");
  Serial.print("The pressure is ");
  Serial.print(pressure);
  Serial.println(" psi");
  */
  Serial.print("Target voltage: ");
  Serial.print(Setpoint*5/1023);
  Serial.println(" V");
  Serial.print("Sensor voltage: ");
  Serial.print(voltage*5/1023);
  Serial.println(" V");
  delay(1000);
}

