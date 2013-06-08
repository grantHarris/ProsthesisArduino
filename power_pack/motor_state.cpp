#include <Arduino.h>
#include <aJSON.h>
#include <command_processor.h>
#include "motor_state.h"
#include "motors.h"

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
    static const char *kPressureSetPoint = "Ps";
    static const char *kProportionalTuning = "P";
    static const char *kIntegralTuning = "I";
    static const char *kDifferentialTuning = "D";
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
  
#if OUTPUT_PID_TUNINGS
  aJsonObject *motorPTunings = aJson.createArray();    
  aJsonObject *motorITunings = aJson.createArray();    
  aJsonObject *motorDTunings = aJson.createArray();      
#endif
  
  for (int i = 0; i < NUM_MOTOR_CONTROLLERS; ++i)
  {
    aJson.addItemToArray(motorCurrents, aJson.createItem((int)state.Currents[i]));
    aJson.addItemToArray(motorVoltages, aJson.createItem(state.Millivolts[i]));
    aJson.addItemToArray(motorOutputPressures, aJson.createItem((int)state.OutputPressure[i]));
    aJson.addItemToArray(motorLoadPressures, aJson.createItem((int)state.LoadPressure[i]));
    aJson.addItemToArray(motorFlowRates, aJson.createItem((int)state.FlowRate[i]));            
    aJson.addItemToArray(motorDutyCycles, aJson.createItem((int)(state.MotorDutyCycle[i] * 100.0f)));     

#if OUTPUT_PID_TUNINGS
    aJson.addItemToArray(motorPTunings, aJson.createItem(state.PTuning[i])); 
    aJson.addItemToArray(motorITunings, aJson.createItem(state.ITuning[i])); 
    aJson.addItemToArray(motorDTunings, aJson.createItem(state.DTuning[i])); 
#endif    
  }
  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kUsingLoadSenseKey, aJson.createItem(state.IsLoadSense));
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kDeviceStateKey, aJson.createItem(state.State));  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kPressureSetPoint, aJson.createItem((int)state.PressureSetPoint));  
  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kCurrentArray, motorCurrents);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kMotorVoltageArray, motorVoltages);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kOutputPressureArray, motorOutputPressures);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kLoadPressureArray, motorLoadPressures);  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kFlowRateArray, motorFlowRates);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kMotorDutyCycleArray, motorDutyCycles);  
  
#if OUTPUT_PID_TUNINGS
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kProportionalTuning, motorPTunings);
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kIntegralTuning, motorITunings);  
  aJson.addItemToObject(msg, MotorState::MotorStateKeys::kDifferentialTuning, motorDTunings);  
#endif  
  
  return msg; 
}

