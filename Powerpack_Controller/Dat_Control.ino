/*
Basic Controlling Code
November 23 2012
Thomas Watanabe
Acknowledgements: PID Library by Brett Beauregard
*/

#include <PID_v1.h>
#include <Motor_Controller.h>
#include <MsTimer2.h>

MotorController MyController(0, 10, 2, 3, 4, 5, 0, 0, 0, 10, 0, 0, 0);

void setup() {
  Serial.begin(9600);

  Serial.println("PID Code");

  delay(2000);
  
  MyController.Initialize();
  MsTimer2::set(100, Control_10Hz);
  MsTimer2::start()
}

void Control_10Hz()
{
  MyController.Iterate();
}

void loop() 
{
  //button and serial write stuff
}
