//#define DEBUG_CBS 1
#include <Arduino.h>
#include <aJSON.h>
#include <command_processor.h>
#include "DeviceState.h"

//Handy wrapper for using stream-like serial printing
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

typedef struct {
  DeviceStates deviceState;
  double x;
} tTelemetryData;

void TransitionToState(DeviceStates toState);
void DeviceDisabledSlice();
void DeviceActiveSlice();
void DeviceFaultSlice();

const char* kArduinoID = "mcon";

aJsonStream mJSONSerialStream(&Serial);
unsigned long mLastTelem;
unsigned long mLastMillis;
unsigned long mActiveMillis;
int mTelemetryPeriodMS;
bool mTelemetryEnabled = false;

tTelemetryData mDeviceState;

void setup()
{
  CommandProcessor::SetTypeIDRequestCallback(IDRequestCallback);
  CommandProcessor::SetTelemetryRequestCallback(TelemetryEnableCb);
  CommandProcessor::SetEnableToggleRequestCallback(InitializeDeviceCb);
  Serial.begin(9600);
  
  mDeviceState.deviceState = Uninitialized;
  mDeviceState.x = 0;
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
    const char *deviceState = mDeviceState.deviceState == Active ? "Enabled" : "Disabled";
    //Serial << "Telem. Diff is: " << millis() - mLastTelem << "ms. Device is " << deviceState << "\n";    
    
    aJsonObject *msg = aJson.createObject();
    if (msg != NULL)
    {
      aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kCommandID , aJson.createItem(CommandProcessor::CommandIDs::kTelemetryID));
      aJson.addItemToObject(msg, "x", aJson.createItem(mDeviceState.x));
      aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kDeviceState , aJson.createItem(mDeviceState.deviceState));
      CommandProcessor::SendMessage(msg);
      aJson.deleteItem(msg);
    }
    mLastTelem = millis(); 
  }
  
  switch (mDeviceState.deviceState)
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
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kDeviceState, aJson.createItem(mDeviceState.deviceState));
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
     const char *deviceState = mDeviceState.deviceState == Active ? "Enabled" : "Disabled";
     Serial << "Setting telemetry period to " << mTelemetryPeriodMS << "ms. Telemetry is " << enableText << ". Device is " << deviceState << "\n";
#endif
  }
  return NULL;
}

aJsonObject * InitializeDeviceCb(aJsonObject *msg, bool enable)
{
  aJsonObject *enableValue = aJson.getObjectItem(msg, CommandProcessor::PacketKeys::kEnable);
  
  if (enableValue != NULL && mDeviceState.deviceState != Fault)
  {
    if (enableValue->valuebool)
    {
      TransitionToState(Active);
    }
    else
    {
      TransitionToState(Disabled);
    } 
  }
  
#if DEBUG_CBS
  Serial << "Device initialise " << enable << "\n";
#endif

  return NULL;
}

void TransitionToState(DeviceStates toState)
{
  //TODO: Perform state transitions here
  DeviceStates fromState = mDeviceState.deviceState;
  aJsonObject *msg = NULL;
  bool successful = false;
  switch (fromState)
  {
  case Uninitialized:
    if (toState != Uninitialized)
    {
      successful = true; 
    }
    break;
    
  case Disabled:
    if (toState != Uninitialized)
    {
      successful = true; 
    }
    break;
  
  case Active:
    if (toState != Uninitialized)
    {
      successful = true; 
    }  
    break;
    
  //No recovery from faults
  case Fault:
    break;
  } 
  
  if (successful)
  {
    msg = aJson.createObject();
    aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kCommandID, aJson.createItem(CommandProcessor::CommandIDs::kStateChange));
    aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kChangeFrom , aJson.createItem(mDeviceState.deviceState));
    aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kChangeTo , aJson.createItem(toState));
    mDeviceState.deviceState = toState;    
  }
  
  if (msg != NULL)
  {
    CommandProcessor::SendMessage(msg);
    aJson.deleteItem(msg);
  }
}

void DeviceDisabledSlice()
{
  mLastMillis = millis();
}

void DeviceActiveSlice()
{
  unsigned long currMillis = millis();
  mActiveMillis += currMillis - mLastMillis;
  
  mDeviceState.x = sin((float)mActiveMillis / 1000.0f);
  
  mLastMillis = currMillis;
}

void DeviceFaultSlice()
{
  mLastMillis = millis();  
}

