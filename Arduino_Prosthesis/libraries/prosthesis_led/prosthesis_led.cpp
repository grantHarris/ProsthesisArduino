#include <Arduino.h>
#include <Wire.h>
#include "prosthesis_led.h"

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_MODE2 0x1
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
#defien MAX_LED_CHANNELS 5

namespace Prosthesis
{
  ProsthesisLEDBoard::ProsthesisLEDBoard(uint8_t address)
   : mAddress(address)
  {
   
  }
  
  void ProsthesisLEDBoard::Initialize(void) 
  {
   Wire.begin();
   Reset();
  }
  
  
  void ProsthesisLEDBoard::Reset(void) 
  {
   Write8(PCA9685_MODE1, 0x0);
   Write8(PCA9685_MODE2, 0b00000100);
  }  
  
  void ProsthesisLEDBoard::SetPWMFreq(float freq) 
  {
    //Serial.print("Attempting to set freq ");
    //Serial.println(freq);
    
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -= 1;
    //Serial.print("Estimated pre-scale: "); Serial.println(prescaleval);
    uint8_t prescale = floor(prescaleval + 0.5);
   // Serial.print("Final pre-scale: "); Serial.println(prescale);  
    
    uint8_t oldmode = Read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
    Write8(PCA9685_MODE1, newmode); // go to sleep
    Write8(PCA9685_PRESCALE, prescale); // set the prescaler
    Write8(PCA9685_MODE1, oldmode);
    delay(5);
    Write8(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
                                            // This is why the beginTransmission below was not working.
    //  Serial.print("Mode now 0x"); Serial.println(read8(PCA9685_MODE1), HEX);
  }
  
  void ProsthesisLEDBoard::SetColour(uint8_t channel, uint16_t r, uint16_t g, uint16_t b)
  {
    if (MAX_LED_CHANNELS <= channel)
    {
        return;
    }
    SetPWM(channel * 3, 0, r);
    SetPWM(channel * 3 + 1, 0, g);
    SetPWM(channel * 3 + 2, 0, b);    
  }
 
  void ProsthesisLEDBoard::SetPWM(uint8_t num, uint16_t on, uint16_t off) 
  {
    //Serial.print("Setting PWM "); Serial.print(num); Serial.print(": "); Serial.print(on); Serial.print("->"); Serial.println(off);
  
    Wire.beginTransmission(mAddress);
    Wire.write(LED0_ON_L+4*num);
    Wire.write(on);
    Wire.write(on>>8);
    Wire.write(off);
    Wire.write(off>>8);
    Wire.endTransmission();
  }
  
  uint8_t ProsthesisLEDBoard::Read8(uint8_t addr) 
  {
    Wire.beginTransmission(mAddress);
    Wire.write(addr);
    Wire.endTransmission();
  
    Wire.requestFrom((uint8_t)mAddress, (uint8_t)1);
    return Wire.read();
  }
  
  void ProsthesisLEDBoard::Write8(uint8_t addr, uint8_t d) 
  {
    Wire.beginTransmission(mAddress);
    Wire.write(addr);
    Wire.write(d);
    Wire.endTransmission();
  }
}
