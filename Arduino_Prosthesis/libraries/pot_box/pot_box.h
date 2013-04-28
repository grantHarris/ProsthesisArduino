#ifndef __POT_BOX_H__
#define __POT_BOX_H__

typedef void (*tConectionDirtyCB)(void);

class ProsthesisPotBox
{
public:
  ProsthesisPotBox(){}
  ProsthesisPotBox(int interruptID, int interruptPin, int pPin, int iPin, int dPin, int initialP, int initialI, int initialD);
  void SetConnectionDirtyCallback(tConectionDirtyCB cb);
  void AttemptReconnect(int *outIsConnected);
  void GetPID(int *outP, int *outI, int *outD);
  int IsConnected();
  
private:
  int mConnectionDirty;
  int mConnected;
  
  int mLastP;
  int mLastI;
  int mLastD;
  
  int mInterruptID;
  int mInterruptPin;
  int mPPin;
  int mIPin;
  int mDPin;
  tConectionDirtyCB mDirtyCallback;
  static void OnPotBoxConnectionStateChange();
  static ProsthesisPotBox *sPotBox;
};

#endif //__POT_BOX_H__
