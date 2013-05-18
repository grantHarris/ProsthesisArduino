//#define DEBUG_POT_BOX 1

#include <Arduino.h>
#include "pot_box.h"

//Handy wrapper for using stream-like serial printing
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#define NUM_DEBOUNCE_SAMPLES 5

namespace ProsthesisPotBox
{
  int mConnectionDirty = true;
  int mConnected = false;
  
  int mLastP = 0;
  int mLastI = 0;
  int mLastD = 0;
  
  int mInterruptID;
  int mInterruptPin;
  int mPPin;
  int mIPin;
  int mDPin;
  tConectionDirtyCB mDirtyCallback = NULL;
  void OnPotBoxConnectionStateChange();

  void Initialize(int interruptID, int interruptPin, int pPin, int iPin, int dPin, int initialP, int initialI, int initialD)
  {
    mInterruptID = (interruptID);
    mInterruptPin = (interruptPin);
    mPPin = (pPin);
    mIPin = (iPin);
    mDPin = (dPin);
    mLastP = (initialP);
    mLastI = (initialI);
    mLastD = (initialD);
    mDirtyCallback = (NULL);
    pinMode(mInterruptPin, INPUT);
    attachInterrupt(mInterruptID, OnPotBoxConnectionStateChange, CHANGE);
    AttemptReconnect(&mConnected);
  }

  void OnPotBoxConnectionStateChange()
  {
#if DEBUG_POT_BOX
    Serial << "State Change";
#endif
    mConnectionDirty = true;
    //Set connected to false, we must manually reconnected in order to get new values.
    mConnected = false;
    if (mDirtyCallback != NULL)
    {
      mDirtyCallback(); 
    }
  }

  void SetConnectionDirtyCallback(tConectionDirtyCB cb)
  {
    mDirtyCallback = cb;
  }

  void AttemptReconnect(int *outIsConnected)
  {  
    if (mConnectionDirty)
    {
      int numOns = 0;
      int index = 0;
      int newState = false;
      for (index = 0; index < NUM_DEBOUNCE_SAMPLES; ++index)
      {
        if (digitalRead(mInterruptPin) == HIGH)
        {
         numOns++; 
        }
      }
       
      newState = numOns > NUM_DEBOUNCE_SAMPLES / 2;
      
      if (newState != mConnected)
      {
        mConnected = newState; 
      }
      mConnectionDirty = false;
      
      if (outIsConnected != NULL)
      {
        *outIsConnected = mConnected; 
      }
    }
  }

  void GetPID(int *outP, int *outI, int *outD)
  {
    int p = mLastP;
    int i = mLastI;
    int d = mLastD;
    
    if (mConnected)
    {
      p = analogRead(mPPin);
      i = analogRead(mIPin);
      d = analogRead(mDPin);
      
      mLastP = p;
      mLastI = i;
      mLastD = d;
    }
    
    *outP = p;
    *outI = i;
    *outD = d;
  }

  int IsConnected()
  {
    return mConnected; 
  }
}
