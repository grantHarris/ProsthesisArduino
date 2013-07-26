#include <Arduino.h>
#include <Wire.h>
#include <prosthesis_led.h>

Prosthesis::ProsthesisLEDBoard pwm = Prosthesis::ProsthesisLEDBoard(0x40);
Prosthesis::ProsthesisLEDBoard pwm2 = Prosthesis::ProsthesisLEDBoard(0x41);

void setup()
{
  pwm.Initialize();
  pwm.SetPWMFreq(1600);  // This is the maximum PWM frequency
  pwm2.Initialize();
  pwm2.SetPWMFreq(1600);  // This is the maximum PWM frequency
    // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
}

void loop()
{
    // Drive each PWM in a 'wave'
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 5; pwmnum++) {
      pwm.SetColour(pwmnum, i, 0, 0 );    
      pwm2.SetColour(pwmnum, i, 0, 0 );
    }
  }
  
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 5; pwmnum++) {
      pwm.SetColour(pwmnum, 0, i, 0 );    
      pwm2.SetColour(pwmnum, 0, i, 0 );
    }
  }  
  
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 5; pwmnum++) {
      pwm.SetColour(pwmnum, 0, 0, i );    
      pwm2.SetColour(pwmnum, 0, 0, i );
    }
  }    
  
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 5; pwmnum++) {
      pwm.SetColour(pwmnum, i, i, i );    
      pwm2.SetColour(pwmnum, i, i, i );
    }
  }   
}


