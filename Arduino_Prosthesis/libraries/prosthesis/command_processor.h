#ifndef __COMMAND_PROCESSOR_H__
#define __COMMAND_PROCESSOR_H__

class aJsonObject;

typedef aJsonObject *(*tTypeIDRequestCallback)(aJsonObject *);
typedef aJsonObject *(*tTelemetryToggleRequestCallback)(aJsonObject *, bool);
typedef aJsonObject *(*tEnableToggleRequestCallback)(aJsonObject *, bool);
typedef aJsonObject *(*tEnableHeartbeatRequestCallback)(aJsonObject *, bool);

namespace CommandProcessor
{
  namespace PacketKeys
  {
    static const char *kCommandID = "ID";
    static const char *kEnable = "EN";
    static const char *kArduinoID = "AID";
    static const char *kToggleResult = "TGR";
    static const char *kPeriod = "PD";
    static const char *kTelemetryState = "TS";
    static const char *kDeviceState = "DS";
    static const char *kChangeFrom = "FR";
    static const char *kChangeTo = "TO";
  }
 
  namespace CommandIDs
  {
    static const char *kIdentify = "Id";
    static const char *kTelemetryToggle = "Te";
    static const char *kDeviceToggle = "Dt";
    static const char *kAcknowledge = "ACK";
    static const char *kTelemetryID = "Tm";
    static const char *kStateChange = "Sc";
    static const char *kHeartBeat = "Hb";    
  }
  
  void ProcessMessage(aJsonObject *msg);
  void SetTypeIDRequestCallback(tTypeIDRequestCallback req);
  void SetTelemetryRequestCallback(tTelemetryToggleRequestCallback req);
  void SetEnableToggleRequestCallback(tEnableToggleRequestCallback req);
  void SetHeartbeatRequestCallback(tEnableHeartbeatRequestCallback req);
  void SendMessage(aJsonObject *msg);
  
  aJsonObject *CreateCommandAckMessage();
}

#endif //__COMMAND_PROCESSOR_H__
