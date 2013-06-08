#include <Arduino.h>
#include <Wire.h>
#include "eng_iface.h"

#define CLEAR_DISPLAY 0x76 // Command to clear the 7seg display (sparkfun version)

int mDisplayUpdatePeriod = 1000;
int mRockerTickAmount = 50;
int mLastDisplayUpdateMS = 0;

typedef struct tDisplayControl{
  int mDisplayAddress;
  int mRockerUpPin;
  int mRockerDownPin;
  
  tRockerPressedCallback mRockerUpCb;
  tRockerPressedCallback mRockerDownCb;
  tGetDisplayValueCallback mDisplayDataCb;
  
  bool mDisplayInfoDirty;
  
  tDisplayControl(int displayAddress, int rockerUpPin, int rockerDownPin)
  : mDisplayAddress(displayAddress),
  mRockerUpPin(rockerUpPin),
  mRockerDownPin(rockerDownPin),
  mRockerUpCb(NULL),
  mRockerDownCb(NULL),
  mDisplayDataCb(NULL)
  {
    pinMode(mRockerUpPin, INPUT);
    pinMode(mRockerDownPin, INPUT);       
  }
  
  tDisplayControl(){}
} tDisplayControl;

tDisplayControl mLeftDisplay;
tDisplayControl mRightDisplay;

void WriteValueToDisplay(int address, int value)
{
  Wire.beginTransmission(address);
  Wire.write(0x79);
  Wire.write(0x00);
  Wire.write(value/1000);
  value %= 1000;
  Wire.write(value/100);
  value %= 100;
  Wire.write(value/10);
  value %= 10;
  Wire.write(value);
  Wire.endTransmission();
}  

namespace ProsthesisEngineeringInterface
{
  void InitializeInterface(int leftDisplayAddress, int leftRockerUpPin, int leftRockerDownPin, int rightDisplayAddress, int rightRockerUpPin, int rightRockerDownPin)
  {
    mLeftDisplay = tDisplayControl(leftDisplayAddress, leftRockerUpPin, leftRockerDownPin);
    mRightDisplay = tDisplayControl(rightDisplayAddress, rightRockerUpPin, rightRockerDownPin);
    
    Wire.begin();
    Wire.beginTransmission(mLeftDisplay.mDisplayAddress);
    Wire.write(CLEAR_DISPLAY);
    Wire.endTransmission();
    Wire.beginTransmission(mRightDisplay.mDisplayAddress);
    Wire.write(CLEAR_DISPLAY);
    Wire.endTransmission();    
    
    mLastDisplayUpdateMS = millis();  
  }

  void SetRockerTickAmount(int amount)
  {
    mRockerTickAmount = amount;
  }
  
  void SetDisplayUpdateRefreshPeriodMS(int timeMS)
  {
    mDisplayUpdatePeriod = timeMS;
  }  
  
  int mLastLeftRocker = 0;
  int mLastRightRocker = 0;
  
  void Update()
  { 
    //TODO: Read rocker pin inputs
    
    if (millis() - mLastDisplayUpdateMS > mLastDisplayUpdateMS)
    {
      int left = 0;
      int right = 0;
      if (mLeftDisplay.mDisplayDataCb != NULL)
      {
        left = mLeftDisplay.mDisplayDataCb(); 
      }
      
      if (mRightDisplay.mDisplayDataCb != NULL)
      {
        right = mRightDisplay.mDisplayDataCb(); 
      }      
      
      WriteValueToDisplay(mLeftDisplay.mDisplayAddress, left);
      WriteValueToDisplay(mRightDisplay.mDisplayAddress, right);
      mLastDisplayUpdateMS = millis();
    }
  }
  
  void SetLeftDisplayDataCallback(tGetDisplayValueCallback cb)
  {
    mLeftDisplay.mDisplayDataCb = cb;
  }
  
  void SetLeftDisplayRockerUpCallback(tRockerPressedCallback cb)
  {
    mLeftDisplay.mRockerUpCb = cb;
  }
  
  void SetLeftDisplayRockerDownCallback(tRockerPressedCallback cb)
  {
    mLeftDisplay.mRockerDownCb = cb;
  }
  
  void SetRightDisplayDataCallback(tGetDisplayValueCallback cb)
  {
    mRightDisplay.mDisplayDataCb = cb;
  }
  
  void SetRightDisplayRockerUpCallback(tRockerPressedCallback cb)
  {
    mRightDisplay.mRockerUpCb = cb;
  }
  
  void SetRightDisplayRockerDownCallback(tRockerPressedCallback cb)
  {
    mRightDisplay.mRockerDownCb = cb;
  }
}

