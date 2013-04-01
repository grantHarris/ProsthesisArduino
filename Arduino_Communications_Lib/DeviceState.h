#ifndef __DEVICE_STATES_H__
#define __DEVICE_STATES_H__

//An unfortunate workaround for enums which are used in function prototypes. The enum definition must be in a header file :(
enum DeviceStates
{
  Uninitialized = -1,
  Disabled = 0,
  Active = 1,
  Fault = 2
};

#endif //__DEVICE_STATES_H__
