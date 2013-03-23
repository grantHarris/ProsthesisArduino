#ifndef __COMMAND_PROCESSOR_H__
#define __COMMAND_PROCESSOR_H__

class aJsonObject;

typedef aJsonObject *(*tTypeIDRequestCallback)(aJsonObject *);
typedef aJsonObject *(*tTelemetryToggleRequestCallback)(aJsonObject *, bool);
typedef aJsonObject *(*tEnableToggleRequestCallback)(aJsonObject *, bool);

namespace CommandProcessor
{
  namespace PacketKeys
  {
    static const char *kCommandID = "ID";
    static const char *kEnable = "EN";
    static const char *kArduinoID = "Ad";
    static const char *kToggleResult = "Tg";
    static const char *kTelemetryPeriod = "Pd";
  }
 
  namespace CommandIDs
  {
    static const char *kIdentify = "Id";
    static const char *kTelemetryToggle = "Te";
    static const char *kDeviceToggle = "In";
    static const char *kAcknowledge = "ACK";
  }
  
  void ProcessMessage(aJsonObject *msg);
  void SetTypeIDRequestCallback(tTypeIDRequestCallback req);
  void SetTelemetryRequestCallback(tTelemetryToggleRequestCallback req);
  void SetEnableToggleRequestCallback(tEnableToggleRequestCallback req);
  void SendMessage(aJsonObject *msg);
  
  aJsonObject *CreateCommandAckMessage();
}

#endif //__COMMAND_PROCESSOR_H__
