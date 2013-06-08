#ifndef __MOTOR_STATE_H__
#define __MOTOR_STATE_H__

#define OUTPUT_PID_TUNINGS 1

#include "DeviceState.h"

#ifndef NUM_MOTOR_CONTROLLERS
#define NUM_MOTOR_CONTROLLERS 2
#endif //NUM_MOTOR_CONTROLLERS

namespace MotorState
{
  typedef struct {
    float Currents[NUM_MOTOR_CONTROLLERS];
    int Millivolts[NUM_MOTOR_CONTROLLERS];
    float OutputPressure[NUM_MOTOR_CONTROLLERS];
    float LoadPressure[NUM_MOTOR_CONTROLLERS];
    float FlowRate[NUM_MOTOR_CONTROLLERS];
    bool IsLoadSense;
    float MotorDutyCycle[NUM_MOTOR_CONTROLLERS];
    float PressureSetPoint;
#if OUTPUT_PID_TUNINGS
    float PTuning[NUM_MOTOR_CONTROLLERS];
    float ITuning[NUM_MOTOR_CONTROLLERS];
    float DTuning[NUM_MOTOR_CONTROLLERS];
#endif    
    DeviceStates State;
  } tMotorControllerState;
  
  aJsonObject *MotorStateToJSON(tMotorControllerState state);
}

#endif //__MOTOR_STATE_H__


