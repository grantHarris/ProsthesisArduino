#ifndef __POT_BOX_H__
#define __POT_BOX_H__

typedef void (*tConectionDirtyCB)(void);

namespace ProsthesisPotBox
{
  void Initialize(int interruptID, int interruptPin, int pPin, int iPin, int dPin, int initialP, int initialI, int initialD);
  void SetConnectionDirtyCallback(tConectionDirtyCB cb);
  void AttemptReconnect(int *outIsConnected);
  void GetPID(int *outP, int *outI, int *outD);
  int IsConnected();

};

#endif //__POT_BOX_H__
