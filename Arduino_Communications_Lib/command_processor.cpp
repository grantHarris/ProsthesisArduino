#include <aJSON.h>
#include "command_processor.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#define ID_KEY "ID"
#define COMMAND_IDENTIFY "Id"
#define COMMAND_TELEMETRY_TOGGLE "Te"
#define COMMAND_DEVICE_INIT_TOGGLE "In";
#define COMMAND_ACK "ACK"

#define PARAMETER_TOGGLE "En"

static tTypeIDRequestCallback mTypeIDRequestCallback = NULL;
static tTelemetryToggleRequestCallback mTelemetryToggleCallback = NULL;
static tEnableToggleRequestCallback mEnableCallback = NULL;

namespace CommandProcessor
{
  /* Process message like: { "pwm": { "8": 0, "9": 128 } } */
  void ProcessMessage(aJsonObject *msg)
  {
   aJsonObject *id = aJson.getObjectItem(msg, ID_KEY);
   if (id != NULL)
   {
     const char *idVal = id->valuestring;
     aJsonObject *returnMessage = NULL;
     
     //Check for ID interrogation
     if (strncmp(idVal, COMMAND_IDENTIFY, strlen(COMMAND_IDENTIFY)) == 0)
     {
      if (mTypeIDRequestCallback != NULL)
      {
        returnMessage = mTypeIDRequestCallback(msg);
      } 
     }
     //Check for telemetry toggle
     else if (strncmp(idVal, COMMAND_TELEMETRY_TOGGLE, strlen(COMMAND_TELEMETRY_TOGGLE)) == 0)
     {
       if (mTelemetryToggleCallback != NULL)
       {
         aJsonObject *toggleVal = aJson.getObjectItem(msg, PARAMETER_TOGGLE);
         returnMessage = mTelemetryToggleCallback(msg, toggleVal != NULL ? toggleVal->valuebool : false);
       }        
     } 
     //Check for device initialize signal
     else if (strncmp(idVal, COMMAND_DEVICE_INIT_TOGGLE, strlen(COMMAND_DEVICE_INIT_TOGGLE)) == 0)
     {
       if (mEnableCallback != NULL)
       {
         aJsonObject *toggleVal = aJson.getObjectItem(msg, PARAMETER_TOGGLE);
         returnMessage = mEnableCallback(msg, toggleVal != NULL ? toggleVal->valuebool : false);
       }        
     } 
     
     //Fire the return message if one came back
     if (returnMessage != NULL)
     {
       Serial << aJson.print(returnMessage) << "\n";
       aJson.deleteItem(returnMessage); 
     }
   }
  }
  
  void SetTypeIDRequestCallback(tTypeIDRequestCallback req)
  {
    mTypeIDRequestCallback = req;
  }
  
  void SetTelemetryRequestCallback(tTelemetryToggleRequestCallback req)
  {
    mTelemetryToggleCallback = req;
  }
  
  void SetEnableToggleRequestCallback(tEnableToggleRequestCallback req)
  {
    mEnableCallback = req;
  }
  
  aJsonObject *CreateCommandAckMessage()
  {
    //Messages are initialized with the correct ID
    aJsonObject *ackMsg = aJson.createObject();
    aJson.addItemToObject(ackMsg, COMMAND_IDENTIFY, aJson.createItem(COMMAND_ACK));
    return ackMsg;
  }
}
