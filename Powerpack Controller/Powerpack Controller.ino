/*
Basic Controlling Code
November 23 2012
Thomas Watanabe
Acknowledgements: PID Library by Brett Beauregard
*/

#include <PID_v1.h>

//global variables
//pins
int powerswitch = 2;    //hook up powerswitch to digital pin 2
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
double pb = 1230.0; //y intercept if 0V mapped to 0psi
double vhigh = 0.07874; //voltage from pressure sensor at 5000psi, based on 254ohm resitor used to convert
double vlow = 0.0156; //voltage from pressure sensor at 0psi, based on 254ohm resitor used to convert
double voltage;
double pressure;
double sensitivity = 1250; //based on 254ohm resitor used to convert 4-20mA in to 1.0-5.0V for 0-5000psi range
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
  Serial.println("Turn on controller when ready");
  
  

  
//watching to see when switch is flipped
  while(digitalRead(powerswitch) == LOW) {
//   Serial.println("Turn on controller when ready");
  }
  
//switch has been fliped, count down to activating PID control
  if(digitalRead(powerswitch) == HIGH){
  Serial.println("Activating PID control in");
//    delay(200);
  Serial.println("3");
//    delay(1000);
  Serial.println("2");
//    delay(1000);
  Serial.println("1");
//    delay(1000);  
  Serial.println("PID CONTROL ACTIVATING");
   delay(500);  
   
 //turn the PID on
  myPID.SetMode(AUTOMATIC);

  }
  
  //read voltage at high pressure 
  //while(newpoint == true && digitalRead(powerswitch) == LOW){
    //vhigh = analogRead(sensorPin);
  //}
  //Serial.println("Calculating...");
 // sensitivity = (phigh-plow)/(vhigh-vlow);
  //delay(2000);
  //Serial.println("Sensitivity: ");
  //Serial.print(sensitivity*1023/5);
  //Serial.println(" psi/V");
  //Serial.println("Monitoring pressure...");
}

void loop() {
  //Read voltage across resistor to get pressure reading
  volt_sum = 0.0;
  for(int i=0;i<20;i++){
    volt_sum = volt_sum + analogRead(sensorPin);
  }
  //average voltage reading
  voltage = volt_sum/20*5/1023;
  //convert voltage to pressure
  pressure = voltage*2*sensitivity - pb;
  
  //here we set the PID values to the current values of each pot
  //thus, the PID constant values are updated every single loop()
  //if this is slow or unneeded, move this to the setup()
  kp = analogRead(kppot)/50;//desensitized by 50x
  ki = analogRead(kipot)/50;//desensitized by 50x
  kd = analogRead(kdpot)/50;//desensitized by 50x 
  myPID.SetTunings(kp, ki, kd);
  
  //do PID stuff
  Setpoint = analogRead(targetpot)*4.8875/2;//divided by 2 to reduce range to 0-2500psi;;
  Input = pressure;//analogRead(sensorPin);
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
 // Serial.print("Target pressure: ");
 // Serial.print(Setpoint);
 // Serial.println(" psi");
  Serial.print("Sensor pressure: ");
  Serial.println(pressure);
 // Serial.println(" psi");
 // Serial.print("Sensor voltage: ");
 // Serial.print(voltage); 
 // Serial.println(" V");
 // Serial.println();
 // delay(1000);
}

