//#define DEBUG_CBS 1

#include <aJSON.h>
#include <Arduino.h>

#include "command_processor.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

enum DeviceStates
{
  Uninitialized = -1,
  Disabled = 0,
  Active = 1,
  Fault = 2
};

void DeviceDisabledSlice();
void DeviceActiveSlice();
void DeviceFaultSlice();

const char* kArduinoID = "test";

aJsonStream mJSONSerialStream(&Serial);
unsigned long mLastTelem;
int mTelemetryPeriodMS;
bool mTelemetryEnabled = false;

DeviceStates mDeviceState;

void setup()
{
  CommandProcessor::SetTypeIDRequestCallback(IDRequestCallback);
  CommandProcessor::SetTelemetryRequestCallback(TelemetryEnableCb);
  CommandProcessor::SetEnableToggleRequestCallback(InitializeDeviceCb);
  Serial.begin(9600);
  
  mDeviceState = Uninitialized;
  mTelemetryPeriodMS = 50;
  mLastTelem = 0;
}

void loop()
{
   if (mJSONSerialStream.available()) 
   {
     /* First, skip any accidental whitespace like newlines. */
     mJSONSerialStream.skip();
   }

  if (mJSONSerialStream.available()) 
  {
     /* Something real on input, let's take a look. */
     aJsonObject *msg = aJson.parse(&mJSONSerialStream);
     CommandProcessor::ProcessMessage(msg);
     aJson.deleteItem(msg);
  }

  //Soft realtime telemetry. Who cares about missed deadlines for these? The mission critical stuff goes into the interrupt CBs
  if (mTelemetryEnabled && millis() - mLastTelem > mTelemetryPeriodMS)
  {
    const char *deviceState = mDeviceState == Active ? "Enabled" : "Disabled";
    Serial << "Telem. Diff is: " << millis() - mLastTelem << "ms. Device is " << deviceState << "\n";    
    mLastTelem = millis(); 
  }
  
  switch (mDeviceState)
  {
   case Disabled:
    DeviceDisabledSlice();
    break;
   
   case Active:
    DeviceActiveSlice();
    break;
   
   case Fault:
    DeviceFaultSlice();
    break; 
    
   default:
    break;
  }
}

aJsonObject * IDRequestCallback(aJsonObject *msg)
{
  aJsonObject *ackMsg = CommandProcessor::CreateCommandAckMessage();
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kArduinoID, aJson.createItem(kArduinoID));
  
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kTelemetryState, aJson.createItem(mTelemetryEnabled));
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kDeviceState, aJson.createItem(mDeviceState));
  return ackMsg;
}

aJsonObject * TelemetryEnableCb(aJsonObject *msg, bool enable)
{
  //No ACK necessary for this
  mTelemetryEnabled = enable;
  aJsonObject *telemPeriod = aJson.getObjectItem(msg, CommandProcessor::PacketKeys::kTelemetryPeriod);
  if (telemPeriod != NULL)
  {
     mTelemetryPeriodMS = telemPeriod->valueint;
#if DEBUG_CBS
     const char *enableText = mTelemetryEnabled ? "on" : "off";
     const char *deviceState = mDeviceState == Active ? "Enabled" : "Disabled";
     Serial << "Setting telemetry period to " << mTelemetryPeriodMS << "ms. Telemetry is " << enableText << ". Device is " << deviceState << "\n";
#endif
  }
  return NULL;
}

aJsonObject * InitializeDeviceCb(aJsonObject *msg, bool enable)
{
  aJsonObject *enableValue = aJson.getObjectItem(msg, CommandProcessor::PacketKeys::kEnable);
  
  if (enableValue != NULL && mDeviceState != Fault)
  {
    if (enableValue->valuebool)
    {
      mDeviceState = Active;
    }
    else
    {
      mDeviceState = Disabled;
    } 
  }
 
  //Echo initialize toggle back for the sake of example
  aJsonObject *ackMsg = CommandProcessor::CreateCommandAckMessage();
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kToggleResult, aJson.createItem(enable));
  
  return ackMsg;
}

void DeviceDisabledSlice()
{
  
}

void DeviceActiveSlice()
{
  
}

void DeviceFaultSlice()
{
  
}

