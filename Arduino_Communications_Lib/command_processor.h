#ifndef __COMMAND_PROCESSOR_H__
#define __COMMAND_PROCESSOR_H__

class aJsonObject;

typedef aJsonObject *(*tTypeIDRequestCallback)(aJsonObject *);
typedef aJsonObject *(*tTelemetryToggleRequestCallback)(aJsonObject *, bool);
typedef aJsonObject *(*tEnableToggleRequestCallback)(aJsonObject *, bool);


namespace CommandProcessor
{
  void ProcessMessage(aJsonObject *msg);
  void SetTypeIDRequestCallback(tTypeIDRequestCallback req);
  void SetTelemetryRequestCallback(tTelemetryToggleRequestCallback req);
  void SetEnableToggleRequestCallback(tEnableToggleRequestCallback req);
  
  aJsonObject *CreateCommandAckMessage();
}

#endif //__COMMAND_PROCESSOR_H__
