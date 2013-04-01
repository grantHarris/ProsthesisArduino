#include <Arduino.h>
#include <aJSON.h>
#include <command_processor.h>
#include "motor_state.h"

namespace MotorState
{
  //JSON keys
  namespace MotorStateKeys
  {
    static const char *kCurrentArray = "C";
    static const char *kMotorVoltageArray = "V";
    static const char *kOutputPressureArray = "Pout";
    static const char *kLoadPressureArray = "Pload";
    static const char *kFlowRateArray = "Fl";
    static const char *kUsingLoadSenseKey = "Load";
    static const char *kMotorDutyCycleArray = "Dt";
    static const char *kDeviceStateKey = "Ds";
  }  
}

aJsonObject *MotorState::MotorStateToJSON(tMotorControllerState state)
{
  aJsonObject *msg = aJson.createObject();
  aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kCommandID, aJson.createItem(CommandProcessor::CommandIDs::kTelemetryID));
  
  aJsonObject *motorCurrents = aJson.createArray();
  aJsonObject *motorVoltages = aJson.createArray();
  aJsonObject *motorOutputPressures = aJson.createArray();  
  aJsonObject *motorLoadPressures = aJson.createArray();  
  aJsonObject *motorFlowRates = aJson.createArray();    
  aJsonObject *motorDutyCycles = aJson.createArray();    
  
  for (int i = 0; i < NUM_MOTOR_CONTROLLERS; ++i)
  {
     aJson.addItemToArray(motorCurrents, aJson.createItem(state.Currents[i]));
     aJson.addItemToArray(motorVoltages, aJson.createItem(state.Millivolts[i]));
     aJson.addItemToArray(motorOutputPressures, aJson.createItem(state.OutputPressure[i]));
     aJson.addItemToArray(motorLoadPressures, aJson.createItem(state.LoadPressure[i]));
     aJson.addItemToArray(motorFlowRates, aJson.createItem(state.FlowRate[i]));     
     aJson.addItemToArray(motorDutyCycles, aJson.createItem(state.MotorDutyCycle[i]));          
  }
  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kUsingLoadSenseKey, aJson.createItem(state.IsLoadSense));
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kDeviceStateKey, aJson.createItem(state.State));  
  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kCurrentArray, motorCurrents);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kMotorVoltageArray, motorVoltages);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kOutputPressureArray, motorOutputPressures);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kLoadPressureArray, motorLoadPressures);  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kFlowRateArray, motorFlowRates);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kMotorDutyCycleArray, motorDutyCycles);  
  
  return msg; 
}
