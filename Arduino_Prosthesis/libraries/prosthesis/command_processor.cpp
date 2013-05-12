//#define DEBUG_PROCESSOR 1
#include <aJSON.h>

#include "command_processor.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

static tTypeIDRequestCallback mTypeIDRequestCallback = NULL;
static tTelemetryToggleRequestCallback mTelemetryToggleCallback = NULL;
static tEnableToggleRequestCallback mEnableCallback = NULL;
static tEnableHeartbeatRequestCallback mHeartbeatCallback = NULL;

namespace CommandProcessor
{  
  void ProcessMessage(aJsonObject *msg)
  {
   aJsonObject *id = aJson.getObjectItem(msg, PacketKeys::kCommandID);
   if (id != NULL)
   {
     const char *idVal = id->valuestring;
     aJsonObject *returnMessage = NULL;
     
     //Check for ID interrogation
     if (strncmp(idVal, CommandIDs::kIdentify, strlen(CommandIDs::kIdentify)) == 0)
     {
      if (mTypeIDRequestCallback != NULL)
      {
        returnMessage = mTypeIDRequestCallback(msg);
      } 
     }
     //Check for telemetry toggle
     else if (strncmp(idVal, CommandIDs::kTelemetryToggle, strlen(CommandIDs::kTelemetryToggle)) == 0)
     {
       if (mTelemetryToggleCallback != NULL)
       {
         aJsonObject *toggleVal = aJson.getObjectItem(msg, PacketKeys::kEnable);
         returnMessage = mTelemetryToggleCallback(msg, toggleVal != NULL ? toggleVal->valuebool : false);
       }        
     } 
     //Check for device initialize signal
     else if (strncmp(idVal, CommandIDs::kDeviceToggle, strlen(CommandIDs::kDeviceToggle)) == 0)
     {
       if (mEnableCallback != NULL)
       {
         aJsonObject *toggleVal = aJson.getObjectItem(msg, PacketKeys::kEnable);
         returnMessage = mEnableCallback(msg, toggleVal != NULL ? toggleVal->valuebool : false);
       }        
     }     
     //Check for heartbeat enable
     else if (strncmp(idVal, CommandIDs::kHeartBeat, strlen(CommandIDs::kHeartBeat)) == 0)
     {
       if (mHeartbeatCallback != NULL)
       {
         aJsonObject *toggleVal = aJson.getObjectItem(msg, PacketKeys::kEnable);
         returnMessage = mHeartbeatCallback(msg, toggleVal != NULL ? toggleVal->valuebool : false);
       }     
     }
     
     //Fire the return message if one came back
     if (returnMessage != NULL)
     {
#if DEBUG_PROCESSOR
       Serial << "Sending ack\n";
#endif
       //IMPORTANT: Free the buffer after! This call creates a 256byte buffer internally and will allocate all RAM in only a few calls!
       char *outBuff = aJson.print(returnMessage);
       if (outBuff != NULL)
       {
         Serial << outBuff << "\n";
         free(outBuff);
       }
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
  
  void SetHeartbeatRequestCallback(tEnableHeartbeatRequestCallback req)
  {
    mHeartbeatCallback = req;
  }
  
  void SendMessage(aJsonObject *msg)
  {
    if (msg != NULL)
    {
      //IMPORTANT: Free the buffer after! This call creates a 256byte buffer internally and will allocate all RAM in only a few calls!
      char *outMsg = aJson.print(msg);
      if (outMsg != NULL)
      {
        Serial << outMsg << "\n"; 
        free(outMsg);
      }
    }
  }
  
  aJsonObject *CreateCommandAckMessage()
  {
    //Messages are initialized with the correct ID
    aJsonObject *ackMsg = aJson.createObject();
    if (ackMsg != NULL)
    {    
      aJson.addItemToObject(ackMsg, PacketKeys::kCommandID, aJson.createItem(CommandIDs::kAcknowledge));
    }
    else
    {
#if DEBUG_PROCESSOR
      Serial << "Command ack allocation failed \n";
#endif 
    }
    return ackMsg;
  }
}
