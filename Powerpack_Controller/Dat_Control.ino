/*
Basic Controlling Code
November 23 2012
Thomas Watanabe
Acknowledgements: PID Library by Brett Beauregard
*/

#include <PID_v1.h>
#include <Motor_Controller.h>

MotorController MyController(0, 10, 2, 3, 4, 5, 0, 0, 0, 10, 0, 0, 0);

void setup() {
  Serial.begin(9600);

  Serial.println("PID Code");

  delay(2000);

  MyController.Initialize();
}

void loop() { //will replace loop() with more robust 10Hz interrupt later
  MyController.Iterate();
}
