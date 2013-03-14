#include <aJSON.h>
#include <Arduino.h>

#include "command_processor.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#define COMMAND_ACK_PARAMETER_ARDUINO_ID "Ad"
#define COMMAND_PARAMAETER_TELEMETRY_PERIOD "Pd"

const char* kArduinoID = "test";

aJsonStream serial_stream(&Serial);
void processMessage(aJsonObject *msg);

int lastTelem;
int telemetryPeriodMS;
bool telemetryEnabled = false;

void setup()
{
  CommandProcessor::SetTypeIDRequestCallback(IDRequestCallback);
  CommandProcessor::SetTelemetryRequestCallback(TelemetryEnableCb);
  CommandProcessor::SetEnableToggleRequestCallback(InitializeDeviceCb);
  Serial.begin(9600);
  
  telemetryPeriodMS = 50;
  lastTelem = 0;
}

void loop()
{
   if (serial_stream.available()) 
   {
     /* First, skip any accidental whitespace like newlines. */
     serial_stream.skip();
   }

  if (serial_stream.available()) 
  {
     /* Something real on input, let's take a look. */
     aJsonObject *msg = aJson.parse(&serial_stream);
     CommandProcessor::ProcessMessage(msg);
     aJson.deleteItem(msg);
  }

  //Soft realtime telemetry. Who cares about missed deadlines for these? The mission critical stuff goes into the interrupt CBs
  if (telemetryEnabled && millis() - lastTelem > telemetryPeriodMS)
  {
    lastTelem = millis(); 
    Serial << "Telem\n";
  }
}

aJsonObject * IDRequestCallback(aJsonObject *msg)
{
  aJsonObject *ackMsg = CommandProcessor::CreateCommandAckMessage();
  aJson.addItemToObject(ackMsg, COMMAND_ACK_PARAMETER_ARDUINO_ID, aJson.createItem(kArduinoID));
  return ackMsg;
}

aJsonObject * TelemetryEnableCb(aJsonObject *msg, bool enable)
{
  //No ACK necessary for this
  telemetryEnabled = enable;
  aJsonObject *telemPeriod = aJson.getObjectItem(msg, COMMAND_PARAMAETER_TELEMETRY_PERIOD);
  if (telemPeriod != NULL)
  {
     telemetryPeriodMS = telemPeriod->valueint;
  }
  return NULL;
}

aJsonObject * InitializeDeviceCb(aJsonObject *msg, bool enable)
{
  return NULL;
}

