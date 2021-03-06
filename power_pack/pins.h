#ifndef _PINS_H__
#define _PINS_H__

#define DEVICE_ACTIVE_PIN 13

#define DISPLAY_HIP_PRESSURE_ADDRESS 0x70 //This is the default address of the OpenSegment with both solder jumpers open
#define DISPLAY_KNEE_PRESSURE_ADDRESS 0x71

#define MOTOR_KNEE_THROTTLE_PIN 12
#define MOTOR_KNEE_PRESSURE_IN A3
#define MOTOR_KNEE_LOAD_IN A5

#define MOTOR_KNEE_SET_POINT_DOWN_PIN 6
#define MOTOR_KNEE_SET_POINT_UP_PIN 4

#define MOTOR_HIP_THROTTLE_PIN 10 //Pin 11 is damaged. do not use!!
#define MOTOR_HIP_PRESSURE_IN A4
#define MOTOR_HIP_LOAD_IN A6

#define MOTOR_HIP_SET_POINT_DOWN_PIN 7
#define MOTOR_HIP_SET_POINT_UP_PIN 5

#define POT_BOX_P_PIN A0
#define POT_BOX_I_PIN A1
#define POT_BOX_D_PIN A2
#define POT_BOX_INTERRUPT_ID 0
#define POT_BOX_INTERRUPT_PIN 2

#endif //_PINS_H__

