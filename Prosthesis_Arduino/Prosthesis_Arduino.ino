#include "spool_valve.h"

// constants won't change. Used here to 
// set pin numbers:
const int ledPinA =  3;      // the number of the LED pin
const int ledPinB = 9;
const int channelAPin = 0;
const int channelBPin = 1;

const int inputDeadZone = 190;

// Variables will change:
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 10;           // interval at which to blink (milliseconds)

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // set the LED with the ledState of the variable:
    int aVal = analogRead(channelAPin);
    int bVal = analogRead(channelBPin);  
    
    if (aVal <= inputDeadZone)
    {
      aVal = 0;
      bVal -= inputDeadZone;
      bVal = max(0, bVal);
    }
    else if (bVal <= inputDeadZone)
    {
      bVal = 0; 
      aVal -= inputDeadZone;
      aVal = max(0, aVal);
    }
    
    analogWrite(ledPinA, (float)255 * (float)aVal / ((float)1024 - (float)inputDeadZone));
    analogWrite(ledPinB, (float)255 * (float)bVal / ((float)1024 - (float)inputDeadZone));
    
    char msg[64];
    snprintf(msg, 63, "Vals: %d %d", aVal, bVal);
    //Serial.println(msg);
  }
}

