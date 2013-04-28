#include <Arduino.h>
#include "pot_box.h"

#define NUM_DEBOUNCE_SAMPLES 5
ProsthesisPotBox *ProsthesisPotBox::sPotBox = NULL;

ProsthesisPotBox::ProsthesisPotBox(int interruptID, int interruptPin, int pPin, int iPin, int dPin, int initialP, int initialI, int initialD) :
mInterruptID(interruptID),
mInterruptPin(interruptPin),
mPPin(pPin),
mIPin(iPin),
mDPin(dPin),
mLastP(initialP),
mLastI(initialI),
mLastD(initialD),
mDirtyCallback(NULL)
{
  pinMode(mInterruptPin, INPUT);
  attachInterrupt(mInterruptID, OnPotBoxConnectionStateChange, CHANGE);
  sPotBox = this;
}

void ProsthesisPotBox::OnPotBoxConnectionStateChange()
{
  sPotBox->mConnectionDirty = true;
  //Set connected to false, we must manually reconnected in order to get new values.
  sPotBox->mConnected = false;
  if (sPotBox->mDirtyCallback != NULL)
  {
    sPotBox->mDirtyCallback(); 
  }
}

void ProsthesisPotBox::SetConnectionDirtyCallback(tConectionDirtyCB cb)
{
  mDirtyCallback = cb;
}

void ProsthesisPotBox::AttemptReconnect(int *outIsConnected)
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

void ProsthesisPotBox::GetPID(int *outP, int *outI, int *outD)
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

int ProsthesisPotBox::IsConnected()
{
  return mConnected; 
}
