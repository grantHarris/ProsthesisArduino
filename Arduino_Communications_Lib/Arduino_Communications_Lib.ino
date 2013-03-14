#include <aJSON.h>
#include <Arduino.h>

#include "command_processor.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

const char* kArduinoID = "test";

aJsonStream serial_stream(&Serial);
void processMessage(aJsonObject *msg);

#define TELEMETRY_PERIOD_MS 50
int lastTelem;
bool telemetryEnabled = false;

void setup()
{
  CommandProcessor::SetTypeIDRequestCallback(IDRequestCallback);
  CommandProcessor::SetTelemetryRequestCallback(TelemetryEnableCb);
  CommandProcessor::SetEnableToggleRequestCallback(InitializeDeviceCb);
  Serial.begin(9600);
  
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

  if (telemetryEnabled && millis() - lastTelem > TELEMETRY_PERIOD_MS)
  {
    lastTelem = millis(); 
    Serial << "Telem\n";
  }
}

aJsonObject * IDRequestCallback(aJsonObject *msg)
{
  aJsonObject *ackMsg = CommandProcessor::CreateCommandAckMessage();
  aJson.addItemToObject(ackMsg, "Ad", aJson.createItem(kArduinoID));
  return ackMsg;
}

aJsonObject * TelemetryEnableCb(aJsonObject *msg, bool enable)
{
  //No ACK necessary for this
  telemetryEnabled = enable;
  return NULL;
}

aJsonObject * InitializeDeviceCb(aJsonObject *msg, bool enable)
{
  return NULL;
}

