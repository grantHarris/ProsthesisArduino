#ifndef __INTERFACE_H__
#define __INTERFACE_H__



namespace Interface
{
  void OpenDisplayConnections();//run in setup
  
  void WriteValueToDisplay(int address, int value);
  
  void UpdateDisplays(float *getOutputPressure(), float *getPressureSetpoint());
  
  void UpdatePressureSetpoint(float *changeSetpoint(float));
  
  bool mPressureSetpointPressed(false);
  float mSetpointIncr(50.0);
}
#endif //__INTERFACE_H__
